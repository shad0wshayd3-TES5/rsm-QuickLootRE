#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	void Listeners::ScrollHandler(const Event& a_event)
	{
		for (auto iter = a_event; iter; iter = iter->next) {
			const auto& inputEvent = *iter;
			if (!inputEvent.HasIDCode()) {
				continue;
			}

			const auto& idEvent = static_cast<const RE::IDEvent&>(inputEvent);
			auto loot = Loot::GetSingleton();
			switch (inputEvent.GetDevice()) {
			case Device::kMouse:
				switch (idEvent.GetIDCode()) {
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
				switch (idEvent.GetIDCode()) {
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
			const auto& inputEvent = *iter;
			if (!inputEvent.HasIDCode()) {
				continue;
			}

			const auto& idEvent = static_cast<const RE::IDEvent&>(inputEvent);
			if (idEvent.GetEventType() != EventType::kButton) {
				continue;
			}

			const auto& buttonEvent = static_cast<const RE::ButtonEvent&>(idEvent);
			auto controlMap = RE::ControlMap::GetSingleton();
			const auto idCode =
				controlMap ?
					controlMap->GetMappedKey("Activate", buttonEvent.GetDevice()) :
					RE::ControlMap::kInvalid;

			if (buttonEvent.GetIDCode() == idCode) {
				if (buttonEvent.IsHeld() && buttonEvent.HeldDuration() > GetGrabDelay()) {
					auto player = RE::PlayerCharacter::GetSingleton();
					if (player) {
						player->StartGrabObject();
						auto playerControls = RE::PlayerControls::GetSingleton();
						auto activateHandler = playerControls ? playerControls->GetActivateHandler() : nullptr;
						if (activateHandler) {
							activateHandler->SetHeldButtonActionSuccess(player->IsGrabbing());
						}
						auto loot = Loot::GetSingleton();
						loot->Close();
					}
					return;
				} else if (buttonEvent.IsUp()) {
					auto loot = Loot::GetSingleton();
					loot->TakeStack();
					return;
				}
			}
		}
	}
}
