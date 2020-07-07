#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	void Listeners::ScrollHandler(const Event& a_event)
	{
		for (auto iter = a_event; iter; iter = iter->next) {
			auto idEvent = iter->AsIDEvent();
			if (!idEvent) {
				continue;
			}

			auto loot = Loot::GetSingleton();
			switch (idEvent->GetDevice()) {
			case Device::kMouse:
				switch (idEvent->GetIDCode()) {
				case Mouse::kWheelUp:
					loot->ModSelectedIndex(-1.0);
					return;
				case Mouse::kWheelDown:
					loot->ModSelectedIndex(1.0);
					return;
				default:
					break;
				}
				break;
			case Device::kGamepad:
				switch (idEvent->GetIDCode()) {
				case Gamepad::kUp:
					loot->ModSelectedIndex(-1.0);
					return;
				case Gamepad::kDown:
					loot->ModSelectedIndex(1.0);
					return;
				default:
					break;
				}
			default:
				break;
			}
		}
	}

	void Listeners::TakeHandler(const Event& a_event)
	{
		for (auto iter = a_event; iter; iter = iter->next) {
			auto buttonEvent = iter->AsButtonEvent();
			if (!buttonEvent) {
				continue;
			}

			auto controlMap = RE::ControlMap::GetSingleton();
			const auto idCode =
				controlMap ?
					controlMap->GetMappedKey("Activate", buttonEvent->GetDevice()) :
					RE::ControlMap::kInvalid;

			if (buttonEvent->GetIDCode() == idCode) {
				if (buttonEvent->IsHeld() && buttonEvent->HeldDuration() > GetGrabDelay()) {
					auto player = RE::PlayerCharacter::GetSingleton();
					if (player) {
						player->StartGrabObject();
						if (player->IsGrabbing()) {
							auto playerControls = RE::PlayerControls::GetSingleton();
							auto activateHandler = playerControls ? playerControls->GetActivateHandler() : nullptr;
							if (activateHandler) {
								activateHandler->SetHeldButtonActionSuccess(true);
							}
							auto loot = Loot::GetSingleton();
							loot->Close();
						}
					}
					return;
				} else if (buttonEvent->IsUp()) {
					auto loot = Loot::GetSingleton();
					loot->TakeStack();
					return;
				}
			}
		}
	}

	void Listeners::TransferHandler(const Event& a_event)
	{
		for (auto iter = a_event; iter; iter = iter->next) {
			auto buttonEvent = iter->AsButtonEvent();
			if (!buttonEvent) {
				continue;
			}

			auto controlMap = RE::ControlMap::GetSingleton();
			const auto idCode =
				controlMap ?
					controlMap->GetMappedKey("Ready Weapon", buttonEvent->GetDevice()) :
					RE::ControlMap::kInvalid;

			if (buttonEvent->GetIDCode() == idCode &&
				buttonEvent->IsDown()) {
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
