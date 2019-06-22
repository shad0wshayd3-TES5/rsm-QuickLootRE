#include "Hooks.h"

#include "skse64_common/BranchTrampoline.h"  // g_localTrampoline, g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <cassert>  // assert
#include <sstream>  // stringstream
#include <string>  // string
#include <typeinfo>  // typeid
#include <vector>  // vector

#include "SetActivateLabelPerkEntryVisitor.h"  // SetActivateLabelPerkEntryVisitor
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "HookShare.h"  // ReturnType, _RegisterForCanProcess_t

#include "SKSE/API.h"  // GetCrosshairRefEventSource
#include "SKSE/Events.h"  // CrosshairRefEvent
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

			auto loot = LootMenu::GetSingleton();
			if (RE::PlayerCharacter::GetSingleton()->GetGrabbedRef()) {
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
			loot->Register(LootMenu::Scaleform::kOpenContainer);
		}
	};


	class TakeAllOp
	{
	public:
		void operator()()
		{
			auto loot = LootMenu::GetSingleton();
			loot->TakeAllItems();
			loot->Register(LootMenu::Scaleform::kOpenContainer);
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

			RE::BSFixedString& str = input->IsGamepadEnabled() ? inputStrHolder->journal : inputStrHolder->pause;
			if (!a_event || a_event->controlID != str || mm->GameIsPaused()) {
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
					float pressure = a_event->pressure;
					float timer = a_event->timer;
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
			SafeWrite64(vFunc.GetAddress(), GetFnAddr(&Hook_ProcessButton));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(MenuOpenHandlerEx).name());
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
							std::string subStr = dispText.substr(beg + 1, end - beg - 1);
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
			SafeWrite64(vFunc.GetAddress(), GetFnAddr(&Hook_GetCrosshairText));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(TESBoundAnimObjectEx).name());
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
			constexpr std::uintptr_t BASE_ADDR = 0x009935A0;	// 1_5_73
			constexpr std::uintptr_t LEA_HOOK = 0x1F;
			constexpr std::uintptr_t JMP_HOOK = 0x36;
			REL::Offset<std::uintptr_t> funcBase(BASE_ADDR);

			SafeWrite8(funcBase.GetAddress() + LEA_HOOK + 3, 0x00);

			g_branchTrampoline.Write5Branch(funcBase.GetAddress() + JMP_HOOK, GetFnAddr(&Hook_BlockActivation));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(TESObjectREFREx).name());
		}
	};


	void InstallGHeapLeakDetectionCrashFix()
	{
		// E8 ? ? ? ? 48 8B 07 33 D2 48 8B CF FF 10
		constexpr std::uintptr_t BASE_ADDR = 0x00FFFA00;	// 1_5_73
		constexpr std::uintptr_t START = 0x4B;
		constexpr std::uintptr_t END = 0x5C;
		constexpr UInt8 NOP = 0x90;
		REL::Offset<std::uintptr_t> funcBase(BASE_ADDR);

		for (std::uintptr_t i = START; i < END; ++i) {
			SafeWrite8(funcBase.GetAddress() + i, NOP);
		}
		_DMESSAGE("[DEBUG] Installed crash fix for scaleform heap leak detection");
	}


	// TEMPORARY
	bool Hook_LookupCrosshairRefByHandle(RE::RefHandle& a_handle, RE::TESObjectREFRPtr& a_refrOut)
	{
		bool result = RE::TESObjectREFR::LookupByHandle(a_handle, a_refrOut);

		g_crosshairRef = a_refrOut;
		SKSE::CrosshairRefEvent event(a_refrOut);
		SKSE::GetCrosshairRefEventSource()->SendEvent(&event);

		return result;
	}


	RE::TESObjectREFR* GetCurrentCrosshairRef_Hook(RE::StaticFunctionTag*)
	{
		return g_crosshairRef.get();
	}


	void InstallCrosshairDispatchFix()
	{
		REL::Offset<std::uintptr_t> target(0x006B0570 + 0x90);
		g_branchTrampoline.Write5Call(target.GetAddress(), GetFnAddr(Hook_LookupCrosshairRefByHandle));
		_DMESSAGE("[DEBUG] Installed fix for crosshair ref dispatch");
	}
	// TEMPORARY


	bool Cmd_SetQuickLootVariable_Execute(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
	{
		auto strChunk = static_cast<RE::CommandInfo::StringChunk*>(a_scriptData->GetChunk());
		std::string name = strChunk->GetString();

		if (name.length() > 1) {
			RE::CommandInfo::IntegerChunk* intChunk = (RE::CommandInfo::IntegerChunk*)strChunk->GetNext();
			int val = intChunk->GetInteger();

			auto console = RE::ConsoleManager::GetSingleton();
			auto setting = Settings::set(name, val);
			if (setting) {
				LootMenu::GetSingleton()->Register(LootMenu::Scaleform::kSetup);

				if (console && RE::ConsoleManager::IsConsoleMode()) {
					console->Print("> [LootMenu] Set \"%s\" = %s", name.c_str(), setting->getValueAsString().c_str());
				}
			} else {
				if (console && RE::ConsoleManager::IsConsoleMode()) {
					console->Print("> [LootMenu] ERROR: Variable \"%s\" not found.", name.c_str());
				}
			}
		}
		return true;
	}


	void RegisterConsoleCommands()
	{
		using Type = RE::SCRIPT_PARAMETER::Type;

		auto info = RE::CommandInfo::LocateConsoleCommand("TestSeenData");  // Unused
		if (info) {
			static RE::SCRIPT_PARAMETER params[] = {
				{ "Name", Type::kString, 0 },
				{ "Value", Type::kInteger, 0 }
			};
			info->longName = "SetQuickLootVariable";
			info->shortName = "sqlv";
			info->helpText = "Set QuickLoot variables \"sqlv [variable name] [new value]\"";
			info->isRefRequired = false;
			info->SetParameters(params);
			info->execute = &Cmd_SetQuickLootVariable_Execute;
			info->eval = 0;

			_DMESSAGE("[DEBUG] Registered console command: %s (%s)", info->longName, info->shortName);
		} else {
			_ERROR("[ERROR] Failed to register console command!\n");
		}
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
			switch (LootMenu::GetSingleton()->GetPlatform()) {
			case LootMenu::Platform::kPC:
				return strHolder->shout;
			case LootMenu::Platform::kOther:
			default:
				return strHolder->chargeItem;
			}
		case ControlID::kToggleRun:
			return strHolder->toggleRun;
		case ControlID::kAutoMove:
			return strHolder->autoMove;
		case ControlID::kFavorites:
			return strHolder->favorites;
		case ControlID::kNone:
		default:
			if (a_controlID != ControlID::kNone) {
				_ERROR("[ERROR] Invalid control ID (%i)\n", a_controlID);
			}
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
		for (int i = 0, j = 1; j < settings.size(); ++i, ++j) {
			if (settings[i] == settings[j]) {
				_ERROR("[ERROR] %s and %s are mapped to the same key (%s)!", settings[i].key().c_str(), settings[j].key().c_str(), settings[i].c_str());
				LootMenu::QueueMessage(LootMenu::Message::kNoInputLoaded);
				return true;
			}
		}

		return false;
	}


	using Set_t = void(const char* a_str);
	template <class Op>
	bool ApplySetting(HookShare::RegisterForCanProcess_t* a_register, Set_t* a_set, sSetting& a_setting)
	{
		using HookShare::Hook;

		auto strHolder = InputStringHolder::GetSingleton();
		bool result = false;

		if (a_setting == "activate") {
			a_register(Hook::kActivate, &ActivateHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->activate.c_str());
			result = true;
		} else if (a_setting == "readyWeapon") {
			a_register(Hook::kReadyWeapon, &ReadyWeaponHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->readyWeapon.c_str());
			result = true;
		} else if (a_setting == "togglePOV") {
			a_register(Hook::kFirstPersonState, &FirstPersonStateHandlerEx<Op>::Hook_CanProcess);
			a_register(Hook::kThirdPersonState, &ThirdPersonStateHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->togglePOV.c_str());
			g_cameraStateHandlerHooked = true;
			result = true;
		} else if (a_setting == "jump") {
			a_register(Hook::kJump, &JumpHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->jump.c_str());
			result = true;
		} else if (a_setting == "sprint") {
			a_register(Hook::kSprint, &SprintHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->sprint.c_str());
			result = true;
		} else if (a_setting == "sneak") {
			a_register(Hook::kSneak, &SneakHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->sneak.c_str());
			result = true;
		} else if (a_setting == "shout") {
			a_register(Hook::kShout, &ShoutHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->shout.c_str());
			result = true;
		} else if (a_setting == "toggleRun") {
			a_register(Hook::kToggleRun, &ToggleRunHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->toggleRun.c_str());
			result = true;
		} else if (a_setting == "autoMove") {
			a_register(Hook::kAutoMove, &AutoMoveHandlerEx<Op>::Hook_CanProcess);
			a_set(strHolder->autoMove.c_str());
			result = true;
		} else {
			_ERROR("[ERROR] Unrecognized mapping (%s)!", a_setting.c_str());
			result = false;
		}

		return result;
	}
}


namespace Hooks
{
	// TEMPORARY
	bool Register_GetCurrentCrosshairRef_Hook(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("GetCurrentCrosshairRef", "Game", GetCurrentCrosshairRef_Hook);
		return true;
	}
	// TEMPORARY


	void InstallHooks(HookShare::RegisterForCanProcess_t* a_register)
	{
		using HookShare::Hook;

		if (!CheckForMappingConflicts()) {
			if (ApplySetting<NullOp>(a_register, &LootMenu::SetSingleLootMapping, Settings::singleLootModifier)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::singleLootModifier.key().c_str(), Settings::singleLootModifier.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::singleLootModifier.key().c_str(), Settings::singleLootModifier.c_str());
			}

			if (ApplySetting<TakeOp>(a_register, &LootMenu::SetTakeMapping, Settings::takeMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::takeMethod.key().c_str(), Settings::takeMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::takeMethod.key().c_str(), Settings::takeMethod.c_str());
			}

			if (ApplySetting<TakeAllOp>(a_register, &LootMenu::SetTakeAllMapping, Settings::takeAllMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::takeAllMethod.key().c_str(), Settings::takeAllMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::takeAllMethod.key().c_str(), Settings::takeAllMethod.c_str());
			}

			if (ApplySetting<SearchOp>(a_register, &LootMenu::SetSearchMapping, Settings::searchMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::searchMethod.key().c_str(), Settings::searchMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::searchMethod.key().c_str(), Settings::searchMethod.c_str());
			}

			if (!g_activateHandlerHooked) {
				a_register(Hook::kActivate, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				_DMESSAGE("[DEBUG] Stubbed activate can process handler");
			}

			if (!g_cameraStateHandlerHooked) {
				a_register(Hook::kFirstPersonState, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				a_register(Hook::kThirdPersonState, &ActivateHandlerEx<NullOp>::Hook_CanProcess);
				_DMESSAGE("[DEBUG] Stubbed camera state can process handlers");
			}
		} else {
			_ERROR("[ERROR] Mapping conflicts detected!");
			_ERROR("[ERROR] No input hooks applied!\n");
		}

		a_register(Hook::kFavorites, &FavoritesHandlerEx<NullOp>::Hook_CanProcess);
		_DMESSAGE("[DEBUG] Stubbed Favorites can process handler");

		if (!Settings::disableActiTextHook) {
			TESObjectACTIEx::InstallHook();
			TESObjectCONTEx::InstallHook();
			TESNPCEx::InstallHook();
		}

		MenuOpenHandlerEx::InstallHook();
		TESObjectREFREx::InstallHook();

		RegisterConsoleCommands();
		InstallGHeapLeakDetectionCrashFix();

		// TEMPORARY
		InstallCrosshairDispatchFix();
		// TEMPORARY
	}
}
