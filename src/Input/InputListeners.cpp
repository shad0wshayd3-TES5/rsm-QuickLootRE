#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	bool ScrollHandler::ProcessKeyboard(const RE::ButtonEvent& a_event)
	{
		using Keyboard = RE::BSWin32KeyboardDevice::Key;

		auto loot = Loot::GetSingleton();
		switch (a_event.GetIDCode()) {
		case Keyboard::kUp:
			loot->ModSelectedIndex(-1.0);
			return true;
		case Keyboard::kDown:
			loot->ModSelectedIndex(1.0);
			return true;
		case Keyboard::kLeft:
		case Keyboard::kPageUp:
			loot->ModSelectedPage(-1.0);
			return true;
		case Keyboard::kRight:
		case Keyboard::kPageDown:
			loot->ModSelectedPage(1.0);
			return true;
		default:
			return false;
		}
	}

	bool ScrollHandler::ProcessMouse(const RE::ButtonEvent& a_event)
	{
		using Mouse = RE::BSWin32MouseDevice::Key;

		auto loot = Loot::GetSingleton();
		switch (a_event.GetIDCode()) {
		case Mouse::kWheelUp:
			loot->ModSelectedIndex(-1.0);
			return true;
		case Mouse::kWheelDown:
			loot->ModSelectedIndex(1.0);
			return true;
		default:
			return false;
		}
	}

	bool ScrollHandler::ProcessGamepad(const RE::ButtonEvent& a_event)
	{
		using Gamepad = RE::BSWin32GamepadDevice::Key;

		auto loot = Loot::GetSingleton();
		switch (a_event.GetIDCode()) {
		case Gamepad::kUp:
			loot->ModSelectedIndex(-1.0);
			return true;
		case Gamepad::kDown:
			loot->ModSelectedIndex(1.0);
			return true;
		case Gamepad::kLeft:
			loot->ModSelectedPage(-1.0);
			return true;
		case Gamepad::kRight:
			loot->ModSelectedPage(1.0);
			return true;
		default:
			return false;
		}
	}

	void TakeHandler::TakeStack()
	{
		auto loot = Loot::GetSingleton();
		loot->TakeStack();
	}

	void TakeHandler::TryGrab()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		player->StartGrabObject();
		if (!player->IsGrabbing()) {
			return;
		}

		auto playerControls = RE::PlayerControls::GetSingleton();
		auto activateHandler = playerControls ? playerControls->GetActivateHandler() : nullptr;
		if (activateHandler) {
			activateHandler->SetHeldButtonActionSuccess(true);
		}

		auto loot = Loot::GetSingleton();
		loot->Close();
	}

	void TransferHandler::DoHandle(RE::InputEvent* const& a_event)
	{
		for (auto iter = a_event; iter; iter = iter->next) {
			auto event = iter->AsButtonEvent();
			if (!event) {
				continue;
			}

			auto controlMap = RE::ControlMap::GetSingleton();
			const auto idCode =
				controlMap ?
					controlMap->GetMappedKey("Ready Weapon", event->GetDevice()) :
					RE::ControlMap::kInvalid;

			if (event->GetIDCode() == idCode && event->IsDown()) {
				auto player = RE::PlayerCharacter::GetSingleton();
				if (player) {
					player->ActivatePickRef();
				}

				auto loot = Loot::GetSingleton();
				loot->Close();
				return;
			}
		}
	}
}
