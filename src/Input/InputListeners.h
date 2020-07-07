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
			_callbacks(),
			_grabDelay(RE::GetINISetting("fZKeyDelay:Controls"))
		{
			_callbacks.emplace_back(&Listeners::TakeHandler);
			_callbacks.emplace_back(&Listeners::ScrollHandler);
			_callbacks.emplace_back(&Listeners::TransferHandler);
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

		using Callback = std::function<void(Listeners&, const Event&)>;

		inline float GetGrabDelay() const
		{
			if (_grabDelay) {
				return _grabDelay->GetFloat();
			} else {
				assert(false);
				return std::numeric_limits<float>::max();
			}
		}

		inline EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>*) override
		{
			if (a_event) {
				for (auto& callback : _callbacks) {
					callback(*this, *a_event);
				}
			}

			return EventResult::kContinue;
		}

		void ScrollHandler(const Event& a_event);
		void TakeHandler(const Event& a_event);
		void TransferHandler(const Event& a_event);

		std::vector<Callback> _callbacks;
		observer<RE::Setting*> _grabDelay;
	};
}
