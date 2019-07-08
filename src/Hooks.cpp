#include "Hooks.h"

#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"

#include <cassert>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "CrosshairHook.h"
#include "LootMenu.h"
#include "SetActivateLabelPerkEntryVisitor.h"
#include "SetQuickLootVariable.h"
#include "Settings.h"

#include "HookShare.h"

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace
{
	template <ControlID controlID, class Op>
	class PlayerInputHandler
	{
	public:
		static HookShare::result_type Hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using HookShare::result_type;
			using EventType = RE::InputEvent::EventType;

			if (a_event->eventType != EventType::kButton) {
				return result_type::kContinue;
			}

			// If the menu closes while the button is still held, input might process when it shouldn't
			auto button = static_cast<RE::ButtonEvent*>(a_event);
			auto loot = LootMenu::GetSingleton();
			if (button->IsRepeating() && loot->ShouldSkipNextInput()) {
				if (button->IsUp()) {
					loot->NextInputSkipped();
				}
				return result_type::kFalse;
			}

			if (loot->IsVisible()) {
				if (button->IsDown() && button->GetControlID() == GetControlID(controlID)) {  // Must be IsDown, otherwise might process input received from another context
					_op();
				}
				return result_type::kFalse;
			}

			return result_type::kContinue;
		}

	private:
		inline static Op _op;
	};


	// Activate handler needs to account for grabbing items
	template <class Op>
	class PlayerInputHandler<ControlID::kActivate, Op>
	{
	public:
		static HookShare::result_type Hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using HookShare::result_type;
			using EventType = RE::InputEvent::EventType;

			auto player = RE::PlayerCharacter::GetSingleton();
			auto loot = LootMenu::GetSingleton();
			if (player->GetGrabbedRef()) {
				loot->Close();
				return result_type::kContinue;
			}

			if (a_event->eventType == EventType::kButton && loot->IsVisible()) {
				auto button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsUp() && button->GetControlID() == GetControlID(ControlID::kActivate)) {  // This must be IsUp, so as to avoid taking an item when grabbing
					_op();
					return result_type::kFalse;
				} else if (button->IsDown()) {  // Inventory menu activation will queue up without this
					return result_type::kFalse;
				}
			}

			return result_type::kContinue;
		}

	private:
		inline static Op _op;
	};


	template <class Op> using FirstPersonStateHandlerEx = PlayerInputHandler<ControlID::kTogglePOV, Op>;
	template <class Op> using ThirdPersonStateHandlerEx = PlayerInputHandler<ControlID::kTogglePOV, Op>;
	template <class Op> using FavoritesHandlerEx = PlayerInputHandler<ControlID::kNone, Op>;
	template <class Op> using SprintHandlerEx = PlayerInputHandler<ControlID::kSprint, Op>;
	template <class Op> using ReadyWeaponHandlerEx = PlayerInputHandler<ControlID::kReadyWeapon, Op>;
	template <class Op> using AutoMoveHandlerEx = PlayerInputHandler<ControlID::kAutoMove, Op>;
	template <class Op> using ToggleRunHandlerEx = PlayerInputHandler<ControlID::kToggleRun, Op>;
	template <class Op> using ActivateHandlerEx = PlayerInputHandler<ControlID::kActivate, Op>;
	template <class Op> using JumpHandlerEx = PlayerInputHandler<ControlID::kJump, Op>;
	template <class Op> using ShoutHandlerEx = PlayerInputHandler<ControlID::kShout, Op>;
	template <class Op> using SneakHandlerEx = PlayerInputHandler<ControlID::kSneak, Op>;


	class TakeOp
	{
	public:
		void operator()()
		{
			auto loot = LootMenu::GetSingleton();
			loot->TakeItemStack();
			Dispatch<OpenContainerDelegate>();
		}
	};


	class TakeAllOp
	{
	public:
		void operator()()
		{
			auto loot = LootMenu::GetSingleton();
			loot->TakeAllItems();
			Dispatch<OpenContainerDelegate>();
		}
	};


	class SearchOp
	{
	public:
		void operator()()
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			player->StartActivation();
		}
	};



	class NullOp
	{
	public:
		void operator()()
		{}
	};


	struct MenuOpenHandlerEx : RE::MenuOpenHandler
	{
	public:
		using func_t = function_type_t<decltype(&RE::MenuOpenHandler::ProcessButton)>;
		inline static func_t* func = 0;


		bool Hook_ProcessButton(RE::ButtonEvent* a_event)
		{
			auto inputStrHolder = RE::InputStringHolder::GetSingleton();
			auto input = RE::InputManager::GetSingleton();
			auto mm = RE::MenuManager::GetSingleton();

			auto& str = input->IsGamepadEnabled() ? inputStrHolder->journal : inputStrHolder->pause;
			if (a_event->controlID != str || mm->GameIsPaused()) {
				return func(this, a_event);
			}

			static bool processed = true;

			bool result = true;
			if (a_event->IsDown()) {
				processed = false;
			} else if (a_event->IsHeld()) {
				if (!processed && a_event->timer >= 2.0) {
					processed = true;
					auto loot = LootMenu::GetSingleton();
					loot->ToggleEnabled();
					loot->QueueMessage(LootMenu::Message::kLootMenuToggled);
				}
			} else {
				if (!processed) {
					auto pressure = a_event->pressure;
					auto timer = a_event->timer;
					a_event->pressure = 1.0;
					a_event->timer = 0.0;
					result = func(this, a_event);
					a_event->pressure = pressure;
					a_event->timer = timer;
					processed = true;
				}
			}

			return result;
		}


		static void InstallHook()
		{
			REL::Offset<func_t**> vFunc(RE::Offset::MenuOpenHandler::Vtbl + (0x5 * 0x8));
			func = *vFunc;
			SafeWrite64(vFunc.GetAddress(), unrestricted_cast<std::uintptr_t>(&Hook_ProcessButton));
			_DMESSAGE("(%s) installed hook", typeid(MenuOpenHandlerEx).name());
		}
	};


	template <std::uintptr_t OFFSET>
	class TESBoundAnimObjectEx : public RE::TESBoundAnimObject
	{
	public:
		using func_t = function_type_t<decltype(&RE::TESBoundAnimObject::GetCrosshairText)>;
		inline static func_t* func = 0;


		bool Hook_GetCrosshairText(RE::TESObjectREFR* a_ref, RE::BSString* a_dst)
		{
			using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;

			auto result = func(this, a_ref, a_dst);
			auto player = RE::PlayerCharacter::GetSingleton();
			auto loot = LootMenu::GetSingleton();
			if (!loot->CanOpen(a_ref, player->IsSneaking())) {
				return result;
			}

			std::stringstream ss(a_dst->c_str());
			std::string dispText;
			if (std::getline(ss, dispText, '\n')) {
				if (!dispText.empty()) {
					if (dispText[0] == '<') {
						int beg = dispText.find_first_of('>');
						int end = dispText.find_last_of('<');
						if (beg != std::string::npos && end != std::string::npos) {
							auto subStr = dispText.substr(beg + 1, end - beg - 1);
							loot->SetActiText(subStr.c_str());
						}
					} else {
						loot->SetActiText(dispText.c_str());
					}
				}
			}

			if (player->CanProcessEntryPointPerkEntry(EntryPoint::kSetActivateLabel)) {
				SetActivateLabelPerkEntryVisitor visitor(player, a_ref);
				player->VisitEntryPointPerkEntries(EntryPoint::kSetActivateLabel, visitor);
			}

			return false;
		}


		static void InstallHook()
		{
			REL::Offset<func_t**> vFunc(OFFSET);
			func = *vFunc;
			SafeWrite64(vFunc.GetAddress(), unrestricted_cast<std::uintptr_t>(&Hook_GetCrosshairText));
			_DMESSAGE("(%s) installed hook", typeid(TESBoundAnimObjectEx).name());
		}
	};


	using TESObjectACTIEx = TESBoundAnimObjectEx<RE::Offset::TESObjectACTI::Vtbl + (0x4C * 0x8)>;
	using TESObjectCONTEx = TESBoundAnimObjectEx<RE::Offset::TESObjectCONT::Vtbl + (0x4C * 0x8)>;
	using TESNPCEx = TESBoundAnimObjectEx<RE::Offset::TESNPC::Vtbl + (0x4C * 0x8)>;


	class TESObjectREFREx : public RE::TESObjectREFR
	{
	public:
		void Hook_BlockActivation(RE::ExtraFlags::Flag a_flags, bool a_blocked)
		{
			SetActivationBlocked(a_blocked);

			auto loot = LootMenu::GetSingleton();
			if (loot->IsVisible() && this == loot->GetContainerRef()) {
				loot->Close();
				loot->ClearContainerRef();
			}
		}


		static void InstallHook()
		{
			// 48 89 5C 24 08 57 48 83 EC 20 49 8B 00 BA 00 00 00 80
			constexpr std::uintptr_t BASE_ADDR = 0x009935A0;	// 1_5_80
			constexpr std::uintptr_t LEA_HOOK = 0x1F;
			constexpr std::uintptr_t JMP_HOOK = 0x36;
			REL::Offset<std::uintptr_t> funcBase(BASE_ADDR);

			SafeWrite8(funcBase.GetAddress() + LEA_HOOK + 3, 0x00);

			g_branchTrampoline.Write5Branch(funcBase.GetAddress() + JMP_HOOK, unrestricted_cast<std::uintptr_t>(&Hook_BlockActivation));
			_DMESSAGE("(%s) installed hook", typeid(TESObjectREFREx).name());
		}
	};


	void InstallGHeapLeakDetectionCrashFix()
	{
		// E8 ? ? ? ? 48 8B 07 33 D2 48 8B CF FF 10
		constexpr std::uintptr_t BASE_ADDR = 0x00FFFA00;	// 1_5_80
		constexpr std::uintptr_t START = 0x4B;
		constexpr std::uintptr_t END = 0x5C;
		constexpr UInt8 NOP = 0x90;
		REL::Offset<std::uintptr_t> funcBase(BASE_ADDR);

		for (std::uintptr_t i = START; i < END; ++i) {
			SafeWrite8(funcBase.GetAddress() + i, NOP);
		}
		_DMESSAGE("Installed crash fix for scaleform heap leak detection");
	}


	RE::BSFixedString& GetControlID(ControlID a_controlID)
	{
		static RE::BSFixedString emptyStr = "";

		auto strHolder = RE::InputStringHolder::GetSingleton();
		switch (a_controlID) {
		case ControlID::kActivate:
			return strHolder->activate;
		case ControlID::kReadyWeapon:
			return strHolder->readyWeapon;
		case ControlID::kTogglePOV:
			return strHolder->togglePOV;
		case ControlID::kJump:
			return strHolder->jump;
		case ControlID::kSprint:
			return strHolder->sprint;
		case ControlID::kSneak:
			return strHolder->sneak;
		case ControlID::kShout:
			{
				auto inputManager = RE::InputManager::GetSingleton();
				return inputManager->IsGamepadEnabled() ? strHolder->chargeItem : strHolder->shout;
			}
		case ControlID::kToggleRun:
			return strHolder->toggleRun;
		case ControlID::kAutoMove:
			return strHolder->autoMove;
		case ControlID::kFavorites:
			return strHolder->favorites;
		default:
			_ERROR("Invalid control ID (%i)\n", a_controlID);
			[[fallthrough]] ;
		case ControlID::kNone:
			return emptyStr;
		}
	}


	bool CheckForMappingConflicts()
	{
		std::vector<sSetting> settings;
		settings.push_back(Settings::singleLootModifier);
		settings.push_back(Settings::takeMethod);
		settings.push_back(Settings::takeAllMethod);
		settings.push_back(Settings::searchMethod);
		if (settings.size() < 2) {
			return false;
		}

		std::sort(settings.begin(), settings.end());
		for (std::size_t i = 0, j = 1; j < settings.size(); ++i, ++j) {
			if (settings[i] == settings[j]) {
				_ERROR("%s and %s are mapped to the same key (%s)!", settings[i].key().c_str(), settings[j].key().c_str(), settings[i].c_str());
				LootMenu::QueueMessage(LootMenu::Message::kNoInputLoaded);
				return true;
			}
		}

		return false;
	}


	template <class Op>
	bool ApplySetting(HookShare::RegisterForCanProcess_t* a_register, llvm::function_ref<void(const char*)> a_set, sSetting& a_setting)
	{
		SettingMap<Op> settingMap;
		auto it = settingMap.find(a_setting);
		if (it != settingMap.end()) {
			it->second(a_register, a_set);
			_DMESSAGE("Applied %s hook to (%s)", a_setting.key().c_str(), a_setting.c_str());
			return true;
		} else {
			_ERROR("Unrecognized mapping (%s)!", a_setting.c_str());
			_ERROR("Failed to apply %s hook to (%s)!\n", a_setting.key().c_str(), a_setting.c_str());
			return false;
		}
	}
}


namespace Hooks
{
	void InstallHooks(HookShare::RegisterForCanProcess_t* a_register)
	{
		using HookShare::Hook;

		if (!CheckForMappingConflicts()) {
			ApplySetting<NullOp>(a_register, &LootMenu::SetSingleLootMapping, Settings::singleLootModifier);
			ApplySetting<TakeOp>(a_register, &LootMenu::SetTakeMapping, Settings::takeMethod);
			ApplySetting<TakeAllOp>(a_register, &LootMenu::SetTakeAllMapping, Settings::takeAllMethod);
			ApplySetting<SearchOp>(a_register, &LootMenu::SetSearchMapping, Settings::searchMethod);

			if (!g_activateHandlerHooked) {
				a_register(Hook::kActivate, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				_DMESSAGE("Stubbed activate can process handler");
			}

			if (!g_cameraStateHandlerHooked) {
				a_register(Hook::kFirstPersonState, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				a_register(Hook::kThirdPersonState, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				_DMESSAGE("Stubbed camera state can process handlers");
			}
		} else {
			_ERROR("Mapping conflicts detected!");
			_ERROR("No input hooks applied!\n");
		}

		a_register(Hook::kFavorites, &FavoritesHandlerEx<NullOp>::Hook_CanProcess);
		_DMESSAGE("Stubbed Favorites can process handler");

		if (!Settings::disableActiTextHook) {
			TESObjectACTIEx::InstallHook();
			TESObjectCONTEx::InstallHook();
			TESNPCEx::InstallHook();
		}

		MenuOpenHandlerEx::InstallHook();
		TESObjectREFREx::InstallHook();

		SetQuickLootVariable::Register();
		InstallGHeapLeakDetectionCrashFix();

		Temporary::CrosshairHook::InstallHooks();
	}
}
