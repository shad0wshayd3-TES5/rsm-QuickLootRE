#pragma once

namespace Input
{
	class Listeners :
		public RE::BSTEventSink<RE::InputEvent*>
	{
	private:
		using super = RE::BSTEventSink<RE::InputEvent*>;

	public:
		inline Listeners() :
			super(),
			_callbacks()
		{
			_callbacks.emplace_back(TakeHandler);
			_callbacks.emplace_back(ScrollHandler);
		}

		Listeners(const Listeners&) = default;
		Listeners(Listeners&&) = default;

		inline ~Listeners() { Disable(); }

		Listeners& operator=(const Listeners&) = default;
		Listeners& operator=(Listeners&&) = default;

		inline void Enable()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->AddEventSink(this);
			}
		}

		inline void Disable()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->RemoveEventSink(this);
			}
		}

	private:
		using Device = RE::INPUT_DEVICE;
		using Event = RE::InputEvent*;
		using EventResult = RE::BSEventNotifyControl;
		using EventType = RE::INPUT_EVENT_TYPE;
		using Gamepad = RE::BSWin32GamepadDevice::Key;
		using Keyboard = RE::BSWin32KeyboardDevice::Key;
		using Mouse = RE::BSWin32MouseDevice::Key;

		using Callback = std::function<void(const Event&)>;

		static void ScrollHandler(const Event& a_event);
		static void TakeHandler(const Event& a_event);

		inline EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>*) override
		{
			if (a_event) {
				for (auto& callback : _callbacks) {
					callback(*a_event);
				}
			}

			return EventResult::kContinue;
		}

		std::vector<Callback> _callbacks;
	};
}
