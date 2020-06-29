#pragma once

namespace Input
{
	class Listeners : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		inline Listeners() :
			super(),
			_callbacks()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->AddEventSink(this);
			}

			_callbacks.emplace_back(ActivateHandler);
			_callbacks.emplace_back(ScrollWheelHandler);
		}

		Listeners(const Listeners&) = default;
		Listeners(Listeners&&) = default;

		virtual inline ~Listeners()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			if (input) {
				input->RemoveEventSink(this);
			}
		}

		Listeners& operator=(const Listeners&) = default;
		Listeners& operator=(Listeners&&) = default;

	private:
		using super = RE::BSTEventSink<RE::InputEvent*>;

		using Device = RE::INPUT_DEVICE;
		using Event = RE::InputEvent*;
		using EventResult = RE::BSEventNotifyControl;
		using EventType = RE::INPUT_EVENT_TYPE;

		using Callback = std::function<void(const Event&)>;

		static void ActivateHandler(const Event& a_event);
		static void ScrollWheelHandler(const Event& a_event);

		inline virtual EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>*) override
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
