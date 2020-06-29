#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	void Listeners::ActivateHandler(const Event& a_event)
	{
		using Keyboard = RE::BSWin32KeyboardDevice::Key;

		for (auto iter = a_event; iter; iter = iter->next) {
			const auto& event = *iter;
			if (!event.HasIDCode()) {
				continue;
			}

			const auto& idEvent = static_cast<const RE::IDEvent&>(event);
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
			auto idCode = controls->GetMappedKey("Activate", RE::INPUT_DEVICE::kKeyboard);
			if (idEvent.GetIDCode() == idCode) {
				auto loot = Loot::GetSingleton();
				loot->TakeStack();
				return;
			}
		}
	}

	void Listeners::ScrollWheelHandler(const Event& a_event)
	{
		using Mouse = RE::BSWin32MouseDevice::Key;

		for (auto iter = a_event; iter; iter = iter->next) {
			const auto& event = *iter;
			if (!event.HasIDCode()) {
				continue;
			}

			const auto& idEvent = static_cast<const RE::IDEvent&>(event);
			auto loot = Loot::GetSingleton();
			switch (event.GetDevice()) {
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
			default:
				break;
			}
		}
	}
}
