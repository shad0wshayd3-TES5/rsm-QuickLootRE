#pragma once

namespace Input
{
	class Listeners : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		Listeners();
		Listeners(const Listeners&) = default;
		Listeners(Listeners&&) = default;

		virtual ~Listeners();

		Listeners& operator=(const Listeners&) = default;
		Listeners& operator=(Listeners&&) = default;

	private:
		using super = RE::BSTEventSink<RE::InputEvent*>;

		using Device = RE::INPUT_DEVICE;
		using Event = RE::InputEvent*;
		using EventResult = RE::BSEventNotifyControl;
		using EventType = RE::INPUT_EVENT_TYPE;

		using Callback = std::function<void(const Event&)>;

		static void ScrollWheelHandler(const Event& a_event);

		virtual EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>* a_eventSource) override;

		std::vector<Callback> _callbacks;
	};
}
