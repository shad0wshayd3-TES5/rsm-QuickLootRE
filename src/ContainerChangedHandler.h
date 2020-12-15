#pragma once

class ContainerChangedHandler :
	public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
	ContainerChangedHandler() :
		ContainerChangedHandler(RE::ObjectRefHandle{})
	{}

	ContainerChangedHandler(const ContainerChangedHandler&) = default;
	ContainerChangedHandler(ContainerChangedHandler&&) = default;

	ContainerChangedHandler(RE::ObjectRefHandle a_container) :
		_container(a_container)
	{
		Register();
	}

	~ContainerChangedHandler() { Unregister(); }

	ContainerChangedHandler& operator=(const ContainerChangedHandler&) = default;
	ContainerChangedHandler& operator=(ContainerChangedHandler&&) = default;

	void SetContainer(RE::ObjectRefHandle a_container) noexcept { _container = a_container; }

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

private:
	void Register()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		if (source) {
			source->AddEventSink(this);
		}
	}

	void Unregister()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		if (source) {
			source->RemoveEventSink(this);
		}
	}

	RE::ObjectRefHandle _container;
};
