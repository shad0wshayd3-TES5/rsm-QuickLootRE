#include "Input/InputListeners.h"

#include "Loot.h"

namespace Input
{
	Listeners::Listeners() :
		super(),
		_callbacks()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		if (input) {
			input->AddEventSink(this);
		}

		_callbacks.emplace_back(ScrollWheelHandler);
	}

	Listeners::~Listeners()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		if (input) {
			input->RemoveEventSink(this);
		}
	}

	void Listeners::ScrollWheelHandler(const Event& a_event)
	{
		using Mouse = RE::BSWin32MouseDevice::Key;

		for (auto iter = a_event; iter; iter = iter->next) {
			auto& event = *iter;
			if (!event.HasIDCode()) {
				continue;
			}

			auto& idEvent = static_cast<const RE::IDEvent&>(event);
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

	auto Listeners::ProcessEvent(const Event* a_event, [[maybe_unused]] RE::BSTEventSource<Event>* a_eventSource)
		-> EventResult
	{
		if (a_event) {
			for (auto& callback : _callbacks) {
				callback(*a_event);
			}
		}

		return EventResult::kContinue;
	}
}
