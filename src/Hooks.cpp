#include "Hooks.h"

#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite64
#include "skse64/GameReferences.h"  // g_thePlayer
#include "skse64/GameTypes.h"  // BSString
#include "skse64/ObScript.h"  // ObScriptCommand

#include <string>  // string
#include <sstream>  // stringstream
#include <typeinfo>  // typeid

#include "Events.h"  // skipCount
#include "SetActivateLabelPerkEntryVisitor.h"  // SetActivateLabelPerkEntryVisitor
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "HookShare.h"  // ReturnType, _RegisterHook_t

#include "RE/ActivateHandler.h"  // ActivateHandler
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/CommandTable.h"  // CommandInfo
#include "RE/ConsoleManager.h"  // ConsoleManager
#include "RE/FavoritesHandler.h"  // FavoritesHandler
#include "RE/InputEvent.h"  // InputEvent
#include "RE/InputStringHolder.h"  // InputStringHolder
#include "RE/MenuManager.h"  // MenuManager
#include "RE/Offsets.h"
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/PlayerControls.h"  // PlayerControls, PlayerControls::Data024
#include "RE/PlayerInputHandler.h"  // PlayerInputHandler
#include "RE/ReadyWeaponHandler.h"  // ReadyWeaponHandler
#include "RE/TESBoundAnimObject.h"  // TESObjectACTI
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/UIManager.h"  // UIManager
#include "RE/UIStringHolder.h"  // UIStringHolder


namespace Hooks
{
	template <typename Op, ControlID controlID>
	class PlayerInputHandler
	{
	public:
		static HookShare::ReturnType hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;
			using HookShare::ReturnType;
			typedef	RE::InputEvent::EventType EventType;

			if (a_event->eventType != EventType::kEventType_Button) {
				return ReturnType::kReturnType_Continue;
			}

			// If the menu closes while the button is still held, input might process when it shouldn't
			RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
			if (button->IsRepeating() && LootMenu::ShouldSkipNextInput()) {
				if (button->IsUp()) {
					LootMenu::NextInputSkipped();
				}
				return ReturnType::kReturnType_False;
			}

			if (QuickLootRE::LootMenu::IsVisible()) {
				if (button->IsDown() && button->GetControlID() == GetControlID(controlID)) {  // Must be IsDown, otherwise might process input received from another context
					Op::Run();
				}
				return ReturnType::kReturnType_False;
			}

			return ReturnType::kReturnType_Continue;
		}
	};


	// Activate handler needs to account for grabbing items
	template <typename Op>
	class PlayerInputHandler<Op, kControlID_Activate>
	{
	public:
		static HookShare::ReturnType hook_CanProcess(RE::PlayerInputHandler* a_this, RE::InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;
			using HookShare::ReturnType;
			typedef	RE::InputEvent::EventType EventType;

			if (RE::PlayerCharacter::GetSingleton()->GetGrabbedRef()) {
				LootMenu::Close();
				return ReturnType::kReturnType_Continue;
			}

			if (a_event->eventType == EventType::kEventType_Button && LootMenu::IsVisible()) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsUp() && button->GetControlID() == GetControlID(kControlID_Activate)) {  // This must be IsUp, so as to avoid taking an item when grabbing
					Op::Run();
					return ReturnType::kReturnType_False;
				} else if (button->IsDown()) {  // Inventory menu activation will queue up without this
					return ReturnType::kReturnType_False;
				}
			}

			return ReturnType::kReturnType_Continue;
		}
	};


#define MAKE_PLAYER_INPUT_HANDLER_EX(TYPE_NAME)														\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_TogglePOV>		FirstPersonStateHandlerEx;	\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_TogglePOV>		ThirdPersonStateHandlerEx;	\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_None>			FavoritesHandlerEx;			\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_Sprint>		SprintHandlerEx;			\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_ReadyWeapon>	ReadyWeaponHandlerEx;		\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_AutoMove>		AutoMoveHandlerEx;			\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_ToggleRun>		ToggleRunHandlerEx;			\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_Activate>		ActivateHandlerEx;			\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_Jump>			JumpHandlerEx;				\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_Shout>			ShoutHandlerEx;				\
	typedef PlayerInputHandler<##TYPE_NAME##, kControlID_Sneak>			SneakHandlerEx;



	class TakeOp
	{
	public:
		static void Run()
		{
			using QuickLootRE::LootMenu;

			LootMenu::GetSingleton()->TakeItemStack();
			LootMenu::Register(LootMenu::kScaleform_OpenContainer);
		}


		MAKE_PLAYER_INPUT_HANDLER_EX(TakeOp);
	};


	class TakeAllOp
	{
	public:
		static void Run()
		{
			using QuickLootRE::LootMenu;

			LootMenu::GetSingleton()->TakeAllItems();
			LootMenu::Register(LootMenu::kScaleform_OpenContainer);
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


	template <uintptr_t offset>
	class TESBoundAnimObjectEx : public RE::TESBoundAnimObject
	{
	public:
		typedef bool(TESBoundAnimObjectEx::*_GetCrosshairText_t)(RE::TESObjectREFR* a_ref, BSString* a_dst, bool a_unk);
		static _GetCrosshairText_t orig_GetCrosshairText;


		bool hook_GetCrosshairText(RE::TESObjectREFR* a_ref, BSString* a_dst, bool a_unk)
		{
			typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

			using QuickLootRE::LootMenu;
			using QuickLootRE::SetActivateLabelPerkEntryVisitor;

			bool result = (this->*orig_GetCrosshairText)(a_ref, a_dst, a_unk);

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			if (LootMenu::CanOpen(a_ref, player->IsSneaking())) {
				std::stringstream ss(a_dst->Get());
				std::string dispText;
				if (std::getline(ss, dispText, '\n')) {
					if (!dispText.empty()) {
						if (dispText[0] == '<') {
							int beg = dispText.find_first_of('>');
							int end = dispText.find_last_of('<');
							if (beg != std::string::npos && end != std::string::npos) {
								std::string subStr = dispText.substr(beg + 1, end - beg - 1);
								LootMenu::SetActiText(subStr.c_str());
							}
						} else {
							LootMenu::SetActiText(dispText.c_str());
						}
					}
				}

				if (player->CanProcessEntryPointPerkEntry(EntryPointType::kEntryPoint_Set_Activate_Label)) {
					SetActivateLabelPerkEntryVisitor visitor(player, a_ref);
					player->VisitEntryPointPerkEntries(EntryPointType::kEntryPoint_Set_Activate_Label, visitor);
				}

				return false;

			} else {
				return result;
			}
		}


		static void installHook()
		{
			RelocPtr<_GetCrosshairText_t> vtbl_GetCrosshairText(offset);
			orig_GetCrosshairText = *vtbl_GetCrosshairText;
			SafeWrite64(vtbl_GetCrosshairText.GetUIntPtr(), GetFnAddr(&hook_GetCrosshairText));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(TESBoundAnimObjectEx).name());
		}
	};


	template <uintptr_t offset> typename TESBoundAnimObjectEx<offset>::_GetCrosshairText_t TESBoundAnimObjectEx<offset>::orig_GetCrosshairText;
	typedef TESBoundAnimObjectEx<RE::TES_OBJECT_ACTI_VTBL_META + 0x268> TESObjectACTIEx;
	typedef TESBoundAnimObjectEx<RE::TES_OBJECT_CONT_VTBL_META + 0x268> TESObjectCONTEx;
	typedef TESBoundAnimObjectEx<RE::TES_NPC_VTBL_META + 0x268> TESNPCEx;


	class ActorEx : public RE::Actor
	{
	public:
		bool Hook_IsRunning()
		{
			return this ? IsRunning() : false;
		}


		static void InstallHook()
		{
			RelocAddr<uintptr_t> call_IsRunning(0x002DB800 + 0x22);
			g_branchTrampoline.Write5Call(call_IsRunning.GetUIntPtr(), GetFnAddr(&Hook_IsRunning));
			_DMESSAGE("[DEBUG] (%s) installed hook", typeid(ActorEx).name());
		}
	};


	bool Cmd_SetQuickLootVariable_Execute(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, Script* a_scriptObj, ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
	{
		using QuickLootRE::ISetting;
		using QuickLootRE::Settings;
		using QuickLootRE::LootMenu;

		if (a_scriptData->strLen < 60) {
			RE::CommandInfo::StringChunk* strChunk = (RE::CommandInfo::StringChunk*)a_scriptData->GetChunk();
			std::string name = strChunk->GetString();

			RE::ConsoleManager* console = RE::ConsoleManager::GetSingleton();

			if (name.length() > 1) {
				RE::CommandInfo::IntegerChunk* intChunk = (RE::CommandInfo::IntegerChunk*)strChunk->GetNext();
				int val = intChunk->GetInteger();

				ISetting* setting = Settings::set(name, val);
				if (setting) {
					LootMenu::Register(LootMenu::kScaleform_Setup);

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
		typedef RE::SCRIPT_PARAMETER::Type Type;

		RE::CommandInfo* info = RE::CommandInfo::Locate("TestSeenData");  // Unused
		if (info) {
			static RE::SCRIPT_PARAMETER params[] = {
				{ "Name", Type::kType_String, 0 },
				{ "Value", Type::kType_Integer, 0 }
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
			_ERROR("[ERROR] Failed to register console command!\n")
		}
	}


	RE::BSFixedString& GetControlID(ControlID a_controlID)
	{
		using QuickLootRE::LootMenu;

		static RE::BSFixedString emptyStr = "";

		RE::InputStringHolder* strHolder = RE::InputStringHolder::GetSingleton();

		switch (a_controlID) {
		case kControlID_Activate:
			return strHolder->activate;
		case kControlID_ReadyWeapon:
			return strHolder->readyWeapon;
		case kControlID_TogglePOV:
			return strHolder->togglePOV;
		case kControlID_Jump:
			return strHolder->jump;
		case kControlID_Sprint:
			return strHolder->sprint;
		case kControlID_Sneak:
			return strHolder->sneak;
		case kControlID_Shout:
			switch (LootMenu::GetPlatform()) {
			case LootMenu::kPlatform_PC:
				return strHolder->shout;
			case LootMenu::kPlatform_Other:
			default:
				return strHolder->chargeItem;
			}
		case kControlID_ToggleRun:
			return strHolder->toggleRun;
		case kControlID_AutoMove:
			return strHolder->autoMove;
		case kControlID_None:
		default:
			if (a_controlID != kControlID_None) {
				_ERROR("[ERROR] Invalid control ID (%i)\n", a_controlID);
			}
			return emptyStr;
		}
	}


	bool CheckForMappingConflicts()
	{
		using QuickLootRE::Settings;
		using QuickLootRE::sSetting;
		using QuickLootRE::LootMenu;

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
				LootMenu::QueueMessage(LootMenu::kMessage_NoInputLoaded);
				return true;
			}
		}

		return false;
	}


	typedef void _Set_t(const char* a_str);
	template <typename T, _Set_t* set>
	bool ApplySetting(HookShare::_RegisterHook_t* a_register, QuickLootRE::sSetting& a_setting)
	{
		using HookShare::Hook;

		InputStringHolder* strHolder = InputStringHolder::GetSingleton();

		if (a_setting == "activate") {
			a_register(T::ActivateHandlerEx::hook_CanProcess, Hook::kHook_Activate);
			set(strHolder->activate.c_str());
			activateHandlerHooked = true;
			return true;
		} else if (a_setting == "readyWeapon") {
			a_register(T::ReadyWeaponHandlerEx::hook_CanProcess, Hook::kHook_ReadyWeapon);
			set(strHolder->readyWeapon.c_str());
			return true;
		} else if (a_setting == "togglePOV") {
			a_register(T::FirstPersonStateHandlerEx::hook_CanProcess, Hook::kHook_FirstPersonState);
			a_register(T::ThirdPersonStateHandlerEx::hook_CanProcess, Hook::kHook_ThirdPersonState);
			set(strHolder->togglePOV.c_str());
			cameraStateHandlerHooked = true;
			return true;
		} else if (a_setting == "jump") {
			a_register(T::JumpHandlerEx::hook_CanProcess, Hook::kHook_Jump);
			set(strHolder->jump.c_str());
			return true;
		} else if (a_setting == "sprint") {
			a_register(T::SprintHandlerEx::hook_CanProcess, Hook::kHook_Sprint);
			set(strHolder->sprint.c_str());
			return true;
		} else if (a_setting == "sneak") {
			a_register(T::SneakHandlerEx::hook_CanProcess, Hook::kHook_Sneak);
			set(strHolder->sneak.c_str());
			return true;
		} else if (a_setting == "shout") {
			a_register(T::ShoutHandlerEx::hook_CanProcess, Hook::kHook_Shout);
			set(strHolder->shout.c_str());
			return true;
		} else if (a_setting == "toggleRun") {
			a_register(T::ToggleRunHandlerEx::hook_CanProcess, Hook::kHook_ToggleRun);
			set(strHolder->toggleRun.c_str());
			return true;
		} else if (a_setting == "autoMove") {
			a_register(T::AutoMoveHandlerEx::hook_CanProcess, Hook::kHook_AutoMove);
			set(strHolder->autoMove.c_str());
			return true;
		} else {
			_ERROR("[ERROR] Unrecognized mapping (%s)!", a_setting.c_str());
			return false;
		}
	}


	void InstallHooks(HookShare::_RegisterHook_t* a_register)
	{
		using QuickLootRE::LootMenu;
		using QuickLootRE::Settings;
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
				a_register(NullOp::ActivateHandlerEx::hook_CanProcess, Hook::kHook_Activate);
				_DMESSAGE("[DEBUG] Stubbed activate handler");
			}

			if (!cameraStateHandlerHooked) {
				a_register(NullOp::ActivateHandlerEx::hook_CanProcess, Hook::kHook_FirstPersonState);
				a_register(NullOp::ActivateHandlerEx::hook_CanProcess, Hook::kHook_ThirdPersonState);
				_DMESSAGE("[DEBUG] Stubbed camera state handlers");
			}
		} else {
			_ERROR("[ERROR] Mapping conflicts detected!");
			_ERROR("[ERROR] No input hooks applied!\n");
		}

		a_register(NullOp::FavoritesHandlerEx::hook_CanProcess, Hook::kHook_Favorites);
		_DMESSAGE("[DEBUG] Stubbed Favorites handler");

		if (!Settings::disableActiTextHook) {
			TESObjectACTIEx::installHook();
			TESObjectCONTEx::installHook();
			TESNPCEx::installHook();
		}

		ActorEx::InstallHook();

		RegisterConsoleCommands();
	}
}
