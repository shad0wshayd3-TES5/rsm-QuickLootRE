#pragma once

class ContainerChangedHandler :
	public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
	ContainerChangedHandler() = default;
	ContainerChangedHandler(const ContainerChangedHandler&) = default;
	ContainerChangedHandler(ContainerChangedHandler&&) = default;

	inline ContainerChangedHandler(RE::ObjectRefHandle a_container) :
		_container(a_container)
	{
		Register();
	}

	inline ~ContainerChangedHandler() { Unregister(); }

	ContainerChangedHandler& operator=(const ContainerChangedHandler&) = default;
	ContainerChangedHandler& operator=(ContainerChangedHandler&&) = default;

	inline void SetContainer(RE::ObjectRefHandle a_container) { _container = a_container; }

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

private:
	inline void Register()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		if (source) {
			source->AddEventSink(this);
		}
	}

	inline void Unregister()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		if (source) {
			source->RemoveEventSink(this);
		}
	}

	RE::ObjectRefHandle _container;
};
