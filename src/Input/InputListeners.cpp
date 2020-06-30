#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	void Listeners::ScrollHandler(const Event& a_event)
	{
		using Mouse = RE::BSWin32MouseDevice::Key;
		using Gamepad = RE::BSWin32GamepadDevice::Key;

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
		using Keyboard = RE::BSWin32KeyboardDevice::Key;

		for (auto iter = a_event; iter; iter = iter->next) {
			const auto& inputEvent = *iter;
			if (!inputEvent.HasIDCode()) {
				continue;
			}

			const auto& idEvent = static_cast<const RE::IDEvent&>(inputEvent);
			switch (idEvent.GetEventType()) {
			case RE::INPUT_EVENT_TYPE::kButton:
				{
					const auto& buttonEvent = static_cast<const RE::ButtonEvent&>(idEvent);
					if (!buttonEvent.IsDown()) {
						continue;
					}
				}
				break;
			default:
				continue;
			}

			auto controls = RE::ControlMap::GetSingleton();
			auto idCode = controls->GetMappedKey("Activate", inputEvent.GetDevice());
			if (idEvent.GetIDCode() == idCode) {
				auto loot = Loot::GetSingleton();
				loot->TakeStack();
				return;
			}
		}
	}
}
