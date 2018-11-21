#include "Hooks.h"

#include "skse64/GameInput.h"  // InputStringHolder
#include "skse64/GameReferences.h"  // g_thePlayer
#include "skse64/GameTypes.h"  // BSString
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite64
#include "xbyak/xbyak.h"

#include <string>  // string
#include <sstream>  // stringstream

#include "HaACTITextOverrideVisitor.h"  // HaACTITextOverrideVisitor
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "RE/ActivateHandler.h"  // ActivateHandler
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/FavoritesHandler.h"  // FavoritesHandler
#include "RE/MenuManager.h"  // MenuManager
#include "RE/Offsets.h"
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/PlayerControls.h"  // PlayerControls, PlayerControls::Data024
#include "RE/PlayerInputHandler.h"  // PlayerInputHandler
#include "RE/ReadyWeaponHandler.h"  // ReadyWeaponHandler
#include "RE/TESBoundAnimObject.h"  // TESObjectACTI
#include "RE/TESObjectREFR.h"  // TESObjectREFR

class PlayerCharacter;


namespace Hooks
{
	template <uintptr_t offset, ControlID controlID, typename Op>
	class PlayerInputHandler : public RE::PlayerInputHandler
	{
	public:
		typedef bool(PlayerInputHandler::*_CanProcess_t)(InputEvent* a_event);
		static _CanProcess_t orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;
			static InputStringHolder* strHolder = InputStringHolder::GetSingleton();

			if (a_event && a_event->eventType == InputEvent::kEventType_Button && QuickLootRE::LootMenu::IsVisible()) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsDown() && *button->GetControlID() == GetControlID(controlID)) {	// Must be IsDown, otherwise might process input received from another context
					Op::Run();
				}
				return false;
			}
			return (this->*orig_CanProcess)(a_event);
		}


		static void installHook()
		{
			RelocPtr<_CanProcess_t> vtbl_CanProcess(offset);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	// Activate handler needs to account for grabbing items
	template <typename Op>
	class ActivateHandler : public RE::ActivateHandler
	{
	public:
		typedef bool(ActivateHandler::*_CanProcess_t)(InputEvent* a_event);
		static _CanProcess_t orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;

			static RE::PlayerCharacter*	player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

			if (player->GetGrabbedRef()) {
				LootMenu::Close();
			}

			if (a_event && a_event->eventType == InputEvent::kEventType_Button && LootMenu::IsVisible()) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsUp() && *a_event->GetControlID() == GetControlID(kControlID_Activate)) {	// This must be IsUp, so as to avoid taking an item when grabbing
					Op::Run();
					return false;
				} else if (button->IsDown()) {  // Inventory menu activation will queue up without this
					return false;
				}
			}
			return (this->*orig_CanProcess)(a_event);
		}


		static void installHook()
		{
			RelocPtr<_CanProcess_t> vtbl_CanProcess(ACTIVATE_HANDLER_VTBL_META + 0x10);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	template <uintptr_t offset, ControlID controlID, typename Op> typename PlayerInputHandler<offset, controlID, Op>::_CanProcess_t PlayerInputHandler<offset, controlID, Op>::orig_CanProcess;
	template <typename Op> typename ActivateHandler<Op>::_CanProcess_t ActivateHandler<Op>::orig_CanProcess;
#define MAKE_PLAYER_INPUT_HANDLER_EX(TYPE_NAME)																								\
																																			\
	typedef ActivateHandler<##TYPE_NAME##>																		ActivateHandlerEx;			\
	typedef PlayerInputHandler<READY_WEAPON_HANDLER_VTBL_META + 0x10, kControlID_ReadyWeapon, ##TYPE_NAME##>	ReadyWeaponHandlerEx;		\
	typedef PlayerInputHandler<FIRST_PERSON_STATE_VTBL_META + 0x60, kControlID_TogglePOV, ##TYPE_NAME##>		FirstPersonStateHandlerEx;	\
	typedef PlayerInputHandler<THIRD_PERSON_STATE_VTBL_META + 0x98, kControlID_TogglePOV, ##TYPE_NAME##>		ThirdPersonStateHandlerEx;	\
	typedef PlayerInputHandler<JUMP_HANDLER_VTBL_META + 0x10, kControlID_Jump, ##TYPE_NAME##>					JumpHandlerEx;				\
	typedef PlayerInputHandler<SPRINT_HANDLER_VTBL_META + 0x10, kControlID_Sprint, ##TYPE_NAME##>				SprintHandlerEx;			\
	typedef PlayerInputHandler<SNEAK_HANDLER_VTBL_META + 0x10, kControlID_Sneak, ##TYPE_NAME##>					SneakHandlerEx;				\
	typedef PlayerInputHandler<SHOUT_HANDLER_VTBL_META + 0x10, kControlID_Shout, ##TYPE_NAME##>					ShoutHandlerEx;				\
	typedef PlayerInputHandler<TOGGLE_RUN_HANDLER_VTBL_META + 0x10, kControlID_ToggleRun, ##TYPE_NAME##>		ToggleRunHandlerEx;			\
	typedef PlayerInputHandler<FAVORITES_HANDLER_VTBL_META + 0x10, kControlID_None, ##TYPE_NAME##>				FavoritesHandlerEx;


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
			static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

			player->StartActivation();
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
			using QuickLootRE::HaACTITextOverrideVisitor;
			static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

			bool result = (this->*orig_GetCrosshairText)(a_ref, a_dst, a_unk);

			if (LootMenu::CanOpen(a_ref, player->IsSneaking())) {
				std::stringstream ss(a_dst->Get());
				std::string dispText;
				if (std::getline(ss, dispText, '\n')) {
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

#if 0
				if (player->CanProcessEntryPointPerkEntry(EntryPointType::kEntryPoint_Set_Activate_Label)) {
					HaACTITextOverrideVisitor visitor(player, a_ref);
					player->VisitEntryPointPerkEntries(EntryPointType::kEntryPoint_Set_Activate_Label, visitor);
				}
#endif

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
		}
	};


	template <uintptr_t offset> typename TESBoundAnimObjectEx<offset>::_GetCrosshairText_t TESBoundAnimObjectEx<offset>::orig_GetCrosshairText;
	typedef TESBoundAnimObjectEx<TES_OBJECT_ACTI_VTBL_META + 0x268> TESObjectACTIEx;
	typedef TESBoundAnimObjectEx<TES_OBJECT_CONT_VTBL_META + 0x268> TESObjectCONTEx;
	typedef TESBoundAnimObjectEx<TES_NPC_VTBL_META + 0x268> TESNPCEx;


	BSFixedString& GetControlID(ControlID a_controlID)
	{
		using QuickLootRE::LootMenu;
		static InputStringHolder* strHolder = InputStringHolder::GetSingleton();

		static BSFixedString emptyStr = "";

		switch (a_controlID) {
		case kControlID_None:
			break;
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
		default:
			_ERROR("[ERROR] Invalid control ID (%i)\n", a_controlID);
		}
		return emptyStr;
	}


	bool CheckForMappingConflicts()
	{
		using QuickLootRE::Settings;
		using QuickLootRE::sSetting;

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
				return true;
			}
		}

		return false;
	}


	typedef void _Set_t(const char* a_str);
	template <typename T, _Set_t* set>
	bool ApplyTakeAllSetting(QuickLootRE::sSetting& a_setting)
	{
		InputStringHolder* strHolder = InputStringHolder::GetSingleton();

		if (a_setting == "activate") {
			T::ActivateHandlerEx::installHook();
			activateHandlerHooked = true;
			set(strHolder->activate.c_str());
			return true;
		} else if (a_setting == "readyWeapon") {
			T::ReadyWeaponHandlerEx::installHook();
			set(strHolder->readyWeapon.c_str());
			return true;
		} else if (a_setting == "togglePOV") {
			T::FirstPersonStateHandlerEx::installHook();
			T::ThirdPersonStateHandlerEx::installHook();
			set(strHolder->togglePOV.c_str());
			return true;
		} else if (a_setting == "jump") {
			T::JumpHandlerEx::installHook();
			set(strHolder->jump.c_str());
			return true;
		} else if (a_setting == "sprint") {
			T::SprintHandlerEx::installHook();
			set(strHolder->sprint.c_str());
			return true;
		} else if (a_setting == "sneak") {
			T::SneakHandlerEx::installHook();
			set(strHolder->sneak.c_str());
			return true;
		} else if (a_setting == "shout") {
			T::ShoutHandlerEx::installHook();
			set(strHolder->shout.c_str());
			return true;
		} else if (a_setting == "toggleRun") {
			T::ToggleRunHandlerEx::installHook();
			set(strHolder->toggleRun.c_str());
			return true;
		} else {
			_ERROR("[ERROR] Unrecognized mapping (%s)!", a_setting.c_str());
			return false;
		}
	}


	void InstallHooks()
	{
		using QuickLootRE::LootMenu;
		using QuickLootRE::Settings;

		if (!CheckForMappingConflicts()) {
			if (!ApplyTakeAllSetting<NullOp, &LootMenu::SetSingleLootMapping>(Settings::singleLootModifier)) {
				_ERROR("[ERROR] Failed to single loot hook!\n");
			}
			if (!ApplyTakeAllSetting<TakeOp, &LootMenu::SetTakeMapping>(Settings::takeMethod)) {
				_ERROR("[ERROR] Failed to apply take hook!\n");
			}
			if (!ApplyTakeAllSetting<TakeAllOp, &LootMenu::SetTakeAllMapping>(Settings::takeAllMethod)) {
				_ERROR("[ERROR] Failed to apply take all hook!\n");
			}
			if (!ApplyTakeAllSetting<SearchOp, &LootMenu::SetSearchMapping>(Settings::searchMethod)) {
				_ERROR("[ERROR] Failed to apply search hook!\n");
			}
			if (!activateHandlerHooked) {
				NullOp::ActivateHandlerEx::installHook();
			}
		} else {
			_ERROR("[ERROR] Mapping conflicts detected!\n");
		}

		NullOp::FavoritesHandlerEx::installHook();
		TESObjectACTIEx::installHook();
		TESObjectCONTEx::installHook();
		TESNPCEx::installHook();
	}
}
