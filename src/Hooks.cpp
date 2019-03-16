#include "Hooks.h"

#include "skse64_common/BranchTrampoline.h"  // g_localTrampoline, g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite
#include "xbyak/xbyak.h"  // xbyak

#include <cassert>  // assert
#include <sstream>  // stringstream
#include <string>  // string
#include <typeinfo>  // typeid
#include <vector>  // vector

#include "SetActivateLabelPerkEntryVisitor.h"  // SetActivateLabelPerkEntryVisitor
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "HookShare.h"  // ReturnType, _RegisterForCanProcess_t

#include "RE/Skyrim.h"


namespace Hooks
{
	template <typename Op, ControlID controlID>
	class PlayerInputHandler
	{
	public:
		static HookShare::ReturnType Hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using HookShare::ReturnType;
			using EventType = RE::InputEvent::EventType;

			if (a_event->eventType != EventType::kButton) {
				return ReturnType::kContinue;
			}

			// If the menu closes while the button is still held, input might process when it shouldn't
			RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
			LootMenu* loot = LootMenu::GetSingleton();
			if (button->IsRepeating() && loot->ShouldSkipNextInput()) {
				if (button->IsUp()) {
					loot->NextInputSkipped();
				}
				return ReturnType::kFalse;
			}

			if (loot->IsVisible()) {
				if (button->IsDown() && button->GetControlID() == GetControlID(controlID)) {  // Must be IsDown, otherwise might process input received from another context
					Op::Run();
				}
				return ReturnType::kFalse;
			}

			return ReturnType::kContinue;
		}
	};


	// Activate handler needs to account for grabbing items
	template <typename Op>
	class PlayerInputHandler<Op, ControlID::kActivate>
	{
	public:
		static HookShare::ReturnType Hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using HookShare::ReturnType;
			using EventType = RE::InputEvent::EventType;

			LootMenu* loot = LootMenu::GetSingleton();
			if (RE::PlayerCharacter::GetSingleton()->GetGrabbedRef()) {
				loot->Close();
				return ReturnType::kContinue;
			}

			if (a_event->eventType == EventType::kButton && loot->IsVisible()) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsUp() && button->GetControlID() == GetControlID(ControlID::kActivate)) {  // This must be IsUp, so as to avoid taking an item when grabbing
					Op::Run();
					return ReturnType::kFalse;
				} else if (button->IsDown()) {  // Inventory menu activation will queue up without this
					return ReturnType::kFalse;
				}
			}

			return ReturnType::kContinue;
		}
	};


#define MAKE_PLAYER_INPUT_HANDLER_EX(TYPE_NAME)													\
	using FirstPersonStateHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kTogglePOV>;	\
	using ThirdPersonStateHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kTogglePOV>;	\
	using FavoritesHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kNone>;				\
	using SprintHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kSprint>;				\
	using ReadyWeaponHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kReadyWeapon>;	\
	using AutoMoveHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kAutoMove>;			\
	using ToggleRunHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kToggleRun>;		\
	using ActivateHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kActivate>;			\
	using JumpHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kJump>;					\
	using ShoutHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kShout>;				\
	using SneakHandlerEx = PlayerInputHandler<##TYPE_NAME##, ControlID::kSneak>;


	class TakeOp
	{
	public:
		static void Run()
		{
			LootMenu* loot = LootMenu::GetSingleton();
			loot->TakeItemStack();
			loot->Register(LootMenu::Scaleform::kOpenContainer);
		}


		MAKE_PLAYER_INPUT_HANDLER_EX(TakeOp);
	};


	class TakeAllOp
	{
	public:
		static void Run()
		{
			LootMenu* loot = LootMenu::GetSingleton();
			loot->TakeAllItems();
			loot->Register(LootMenu::Scaleform::kOpenContainer);
		}


		MAKE_PLAYER_INPUT_HANDLER_EX(TakeAllOp);
	};


	class SearchOp
	{
	public:
		static void Run()
		{
			RE::PlayerCharacter::GetSingleton()->StartActivation();
		}


		MAKE_PLAYER_INPUT_HANDLER_EX(SearchOp);
	};



	class NullOp
	{
	public:
		static void Run()
		{}


		MAKE_PLAYER_INPUT_HANDLER_EX(NullOp);
	};


	struct MenuOpenHandlerEx : RE::MenuOpenHandler
	{
	public:
		using _ProcessButton_t = bool(RE::MenuOpenHandler* a_this, RE::ButtonEvent* a_event);
		static _ProcessButton_t* orig_ProcessButton;


		bool Hook_ProcessButton(RE::ButtonEvent* a_event)
		{
			RE::InputStringHolder* inputStrHolder = RE::InputStringHolder::GetSingleton();
			RE::InputManager* input = RE::InputManager::GetSingleton();
			RE::MenuManager* mm = RE::MenuManager::GetSingleton();

			RE::BSFixedString& str = input->IsGamepadEnabled() ? inputStrHolder->journal : inputStrHolder->pause;
			if (!a_event || a_event->controlID != str || mm->GameIsPaused()) {
				return orig_ProcessButton(this, a_event);
			}

			static bool processed = true;

			bool result = true;
			if (a_event->IsDown()) {
				processed = false;
			} else if (a_event->IsHeld()) {
				if (!processed && a_event->timer >= 2.0) {
					processed = true;
					LootMenu* loot = LootMenu::GetSingleton();
					loot->ToggleEnabled();
					loot->QueueMessage(LootMenu::Message::kLootMenuToggled);
				}
			} else {
				if (!processed) {
					float pressure = a_event->pressure;
					float timer = a_event->timer;
					a_event->pressure = 1.0;
					a_event->timer = 0.0;
					result = orig_ProcessButton(this, a_event);
					a_event->pressure = pressure;
					a_event->timer = timer;
					processed = true;
				}
			}

			return result;
		}


		static void InstallHook()
		{
			RelocPtr<_ProcessButton_t*> vtbl_ProcessButton(RE::Offset::MenuOpenHandler::Vtbl + (0x5 * 0x8));
			orig_ProcessButton = *vtbl_ProcessButton;
			SafeWrite64(vtbl_ProcessButton.GetUIntPtr(), GetFnAddr(&Hook_ProcessButton));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(MenuOpenHandlerEx).name());
		}
	};


	MenuOpenHandlerEx::_ProcessButton_t* MenuOpenHandlerEx::orig_ProcessButton;


	template <std::uintptr_t offset>
	class TESBoundAnimObjectEx : public RE::TESBoundAnimObject
	{
	public:
		using _GetCrosshairText_t = bool(RE::TESBoundAnimObject* a_this, RE::TESObjectREFR* a_ref, RE::BSString* a_dst, bool a_unk);
		static _GetCrosshairText_t* orig_GetCrosshairText;


		bool Hook_GetCrosshairText(RE::TESObjectREFR* a_ref, RE::BSString* a_dst, bool a_unk)
		{
			using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;

			bool result = orig_GetCrosshairText(this, a_ref, a_dst, a_unk);

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			LootMenu* loot = LootMenu::GetSingleton();
			if (loot->CanOpen(a_ref, player->IsSneaking())) {
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
			} else {
				return result;
			}
		}


		static void InstallHook()
		{
			RelocPtr<_GetCrosshairText_t*> vtbl_GetCrosshairText(offset);
			orig_GetCrosshairText = *vtbl_GetCrosshairText;
			SafeWrite64(vtbl_GetCrosshairText.GetUIntPtr(), GetFnAddr(&Hook_GetCrosshairText));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(TESBoundAnimObjectEx).name());
		}
	};


	template <std::uintptr_t offset> typename TESBoundAnimObjectEx<offset>::_GetCrosshairText_t* TESBoundAnimObjectEx<offset>::orig_GetCrosshairText;
	using TESObjectACTIEx = TESBoundAnimObjectEx<RE::Offset::TESObjectACTI::Vtbl + (0x4C * 0x8)>;
	using TESObjectCONTEx = TESBoundAnimObjectEx<RE::Offset::TESObjectCONT::Vtbl + (0x4C * 0x8)>;
	using TESNPCEx = TESBoundAnimObjectEx<RE::Offset::TESNPC::Vtbl + (0x4C * 0x8)>;


	class TESObjectREFREx : public RE::TESObjectREFR
	{
	public:
		void Hook_BlockActivation(RE::ExtraFlags::Flag a_flags, bool a_blocked)
		{
			SetActivationBlocked(a_blocked);

			LootMenu* loot = LootMenu::GetSingleton();
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
			RelocAddr<std::uintptr_t> funcBase(BASE_ADDR);

			SafeWrite8(funcBase.GetUIntPtr() + LEA_HOOK + 3, 0x00);

			g_branchTrampoline.Write5Branch(funcBase.GetUIntPtr() + JMP_HOOK, GetFnAddr(&Hook_BlockActivation));
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
		RelocAddr<std::uintptr_t> funcBase(BASE_ADDR);

		for (std::uintptr_t i = START; i < END; ++i) {
			SafeWrite8(funcBase.GetUIntPtr() + i, NOP);
		}
		_DMESSAGE("[DEBUG] Installed crash fix for scaleform heap leak detection");
	}


	bool Cmd_SetQuickLootVariable_Execute(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
	{
		if (a_scriptData->strLen < 60) {
			RE::CommandInfo::StringChunk* strChunk = (RE::CommandInfo::StringChunk*)a_scriptData->GetChunk();
			std::string name = strChunk->GetString();

			RE::ConsoleManager* console = RE::ConsoleManager::GetSingleton();

			if (name.length() > 1) {
				RE::CommandInfo::IntegerChunk* intChunk = (RE::CommandInfo::IntegerChunk*)strChunk->GetNext();
				int val = intChunk->GetInteger();

				ISetting* setting = Settings::set(name, val);
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
		}
		return true;
	}


	void RegisterConsoleCommands()
	{
		using Type = RE::SCRIPT_PARAMETER::Type;

		RE::CommandInfo* info = RE::CommandInfo::Locate("TestSeenData");  // Unused
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

		RE::InputStringHolder* strHolder = RE::InputStringHolder::GetSingleton();

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


	using _Set_t = void(const char* a_str);
	template <typename T, _Set_t* set>
	bool ApplySetting(HookShare::_RegisterForCanProcess_t* a_register, sSetting& a_setting)
	{
		using HookShare::Hook;

		InputStringHolder* strHolder = InputStringHolder::GetSingleton();

		bool result = false;

		if (a_setting == "activate") {
			a_register(T::ActivateHandlerEx::Hook_CanProcess, Hook::kActivate);
			set(strHolder->activate.c_str());
			activateHandlerHooked = true;
			result = true;
		} else if (a_setting == "readyWeapon") {
			a_register(T::ReadyWeaponHandlerEx::Hook_CanProcess, Hook::kReadyWeapon);
			set(strHolder->readyWeapon.c_str());
			result = true;
		} else if (a_setting == "togglePOV") {
			a_register(T::FirstPersonStateHandlerEx::Hook_CanProcess, Hook::kFirstPersonState);
			a_register(T::ThirdPersonStateHandlerEx::Hook_CanProcess, Hook::kThirdPersonState);
			set(strHolder->togglePOV.c_str());
			cameraStateHandlerHooked = true;
			result = true;
		} else if (a_setting == "jump") {
			a_register(T::JumpHandlerEx::Hook_CanProcess, Hook::kJump);
			set(strHolder->jump.c_str());
			result = true;
		} else if (a_setting == "sprint") {
			a_register(T::SprintHandlerEx::Hook_CanProcess, Hook::kSprint);
			set(strHolder->sprint.c_str());
			result = true;
		} else if (a_setting == "sneak") {
			a_register(T::SneakHandlerEx::Hook_CanProcess, Hook::kSneak);
			set(strHolder->sneak.c_str());
			result = true;
		} else if (a_setting == "shout") {
			a_register(T::ShoutHandlerEx::Hook_CanProcess, Hook::kShout);
			set(strHolder->shout.c_str());
			result = true;
		} else if (a_setting == "toggleRun") {
			a_register(T::ToggleRunHandlerEx::Hook_CanProcess, Hook::kToggleRun);
			set(strHolder->toggleRun.c_str());
			result = true;
		} else if (a_setting == "autoMove") {
			a_register(T::AutoMoveHandlerEx::Hook_CanProcess, Hook::kAutoMove);
			set(strHolder->autoMove.c_str());
			result = true;
		} else {
			_ERROR("[ERROR] Unrecognized mapping (%s)!", a_setting.c_str());
			result = false;
		}

		return result;
	}


	void InstallHooks(HookShare::_RegisterForCanProcess_t* a_register)
	{
		using HookShare::Hook;

		if (!CheckForMappingConflicts()) {
			if (ApplySetting<NullOp, &LootMenu::SetSingleLootMapping>(a_register, Settings::singleLootModifier)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::singleLootModifier.key().c_str(), Settings::singleLootModifier.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::singleLootModifier.key().c_str(), Settings::singleLootModifier.c_str());
			}

			if (ApplySetting<TakeOp, &LootMenu::SetTakeMapping>(a_register, Settings::takeMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::takeMethod.key().c_str(), Settings::takeMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::takeMethod.key().c_str(), Settings::takeMethod.c_str());
			}

			if (ApplySetting<TakeAllOp, &LootMenu::SetTakeAllMapping>(a_register, Settings::takeAllMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::takeAllMethod.key().c_str(), Settings::takeAllMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::takeAllMethod.key().c_str(), Settings::takeAllMethod.c_str());
			}

			if (ApplySetting<SearchOp, &LootMenu::SetSearchMapping>(a_register, Settings::searchMethod)) {
				_DMESSAGE("[DEBUG] Applied %s hook to (%s)", Settings::searchMethod.key().c_str(), Settings::searchMethod.c_str());
			} else {
				_ERROR("[ERROR] Failed to apply %s hook to (%s)!\n", Settings::searchMethod.key().c_str(), Settings::searchMethod.c_str());
			}

			if (!activateHandlerHooked) {
				a_register(NullOp::ActivateHandlerEx::Hook_CanProcess, Hook::kActivate);
				_DMESSAGE("[DEBUG] Stubbed activate can process handler");
			}

			if (!cameraStateHandlerHooked) {
				a_register(NullOp::ActivateHandlerEx::Hook_CanProcess, Hook::kFirstPersonState);
				a_register(NullOp::ActivateHandlerEx::Hook_CanProcess, Hook::kThirdPersonState);
				_DMESSAGE("[DEBUG] Stubbed camera state can process handlers");
			}
		} else {
			_ERROR("[ERROR] Mapping conflicts detected!");
			_ERROR("[ERROR] No input hooks applied!\n");
		}

		a_register(NullOp::FavoritesHandlerEx::Hook_CanProcess, Hook::kFavorites);
		_DMESSAGE("[DEBUG] Stubbed Favorites can process handler");

		if (!Settings::disableActiTextHook) {
			TESObjectACTIEx::InstallHook();
			TESObjectCONTEx::InstallHook();
			TESNPCEx::InstallHook();
		}

		MenuOpenHandlerEx::InstallHook();
		TESObjectREFREx::InstallHook();

		RegisterConsoleCommands();

#if _DEBUG
		InstallGHeapLeakDetectionCrashFix();
#endif
	}
}
