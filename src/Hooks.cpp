#include "Hooks.h"

#include "skse64/GameReferences.h"  // g_thePlayer
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite64
#include "xbyak/xbyak.h"

#include "LootMenu.h"  // LootMenu
#include "Offsets.h"

#include "RE/ActivateHandler.h"  // ActivateHandler
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/PlayerControls.h"  // PlayerControls, PlayerControls::Data024
#include "RE/PlayerInputHandler.h"  // PlayerInputHandler
#include "RE/ReadyWeaponHandler.h"  // ReadyWeaponHandler


class PlayerCharacter;


namespace Hooks
{
	RelocAddr<_SendItemsPickPocketedEvent*>	SendItemsPickPocketedEvent(SEND_ITEMS_PICKPOCKETED_EVENT);


	template <uintptr_t offset>
	class CameraStateHandler : public RE::PlayerInputHandler
	{
	public:
		typedef bool(CameraStateHandler::*_CanProcess)(InputEvent* a_event);
		static _CanProcess orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			static InputStringHolder* strHolder = InputStringHolder::GetSingleton();

			bool result = (this->*orig_CanProcess)(a_event);
			if (a_event && result && QuickLootRE::LootMenu::IsVisible()) {
				result = (*a_event->GetControlID() != strHolder->togglePOV);
			}

			return result;
		}


		static void installHook()
		{
			RelocPtr<_CanProcess> vtbl_CanProcess(offset);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	template <uintptr_t offset> typename CameraStateHandler<offset>::_CanProcess CameraStateHandler<offset>::orig_CanProcess;
	typedef CameraStateHandler<FIRST_PERSON_STATE_VTBL_META + 0x60> FirstPersonStateHandler;
	typedef CameraStateHandler<THIRD_PERSON_STATE_VTBL_META + 0x98> ThirdPersonStateHandler;


	class FavoritesHandler : public RE::MenuEventHandler
	{
	public:
		typedef bool(FavoritesHandler::*_CanProcess)(InputEvent* a_event);
		static _CanProcess orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;
			typedef RE::BSWin32GamepadDevice::Gamepad Gamepad;

			bool result = (this->*orig_CanProcess)(a_event);
			if (a_event && result && LootMenu::IsVisible()) {
				if (a_event->deviceType == kDeviceType_Gamepad && a_event->eventType == InputEvent::kEventType_Button) {
					ButtonEvent* button = static_cast<ButtonEvent*>(a_event);
					result = (button->keyMask != Gamepad::kGamepad_Up && button->keyMask != Gamepad::kGamepad_Down);
				}
			}

			return result;
		}


		static void installHook()
		{
			RelocPtr<_CanProcess> vtbl_CanProcess(FAVORITES_HANDLER_VTBL_META + 0x10);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	FavoritesHandler::_CanProcess FavoritesHandler::orig_CanProcess;


	class ReadyWeaponHandlerEx : public RE::ReadyWeaponHandler
	{
	public:
		typedef void(ReadyWeaponHandlerEx::*_ProcessButton)(ButtonEvent* a_event, RE::PlayerControls::Data024* a_data);
		static _ProcessButton orig_ProcessButton;


		void hook_ProcessButton(ButtonEvent* a_event, RE::PlayerControls::Data024* a_data)
		{
			using QuickLootRE::LootMenu;

			static RE::PlayerCharacter*	player		= reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);
			static UIManager*			uiManager	= UIManager::GetSingleton();

			if (LootMenu::IsOpen()) {
				LootMenu::Close();
				LootMenu::ClearContainerRef(false);
				player->StartActivation();
			} else {
				(this->*orig_ProcessButton)(a_event, a_data);
			}
		}


		static void installHook()
		{
			RelocPtr<_ProcessButton> vtbl_ProcessButton(READY_WEAPON_HANDLER_VTBL_META + 0x28);
			orig_ProcessButton = *vtbl_ProcessButton;
			SafeWrite64(vtbl_ProcessButton.GetUIntPtr(), GetFnAddr(&hook_ProcessButton));
		}
	};


	ReadyWeaponHandlerEx::_ProcessButton ReadyWeaponHandlerEx::orig_ProcessButton;


	class ActivateHandlerEx : public RE::ActivateHandler
	{
	public:
		typedef bool(ActivateHandlerEx::*_CanProcess)(InputEvent* a_event);
		static _CanProcess orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			using QuickLootRE::LootMenu;
			static InputStringHolder* strHolder = InputStringHolder::GetSingleton();

			BSFixedString str = *a_event->GetControlID();
			if (str == strHolder->activate || str == strHolder->accept) {
				bool dummy = true;
			}

			if (LootMenu::IsVisible() && a_event && (*a_event->GetControlID() == strHolder->activate) && (a_event->eventType == InputEvent::kEventType_Button)) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);
				if (button->IsDown()) {
					LootMenu::GetSingleton()->TakeItem();
					return false;
				}
			}
			return (this->*orig_CanProcess)(a_event);;
		}


		static void installHook()
		{
			RelocPtr<_CanProcess> vtbl_CanProcess(ACTIVATE_HANDLER_VTBL_META + 0x10);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	ActivateHandlerEx::_CanProcess ActivateHandlerEx::orig_CanProcess;


	void installHooks()
	{
		FirstPersonStateHandler::installHook();
		ThirdPersonStateHandler::installHook();
		FavoritesHandler::installHook();
		ReadyWeaponHandlerEx::installHook();
		ActivateHandlerEx::installHook();
	}
}
