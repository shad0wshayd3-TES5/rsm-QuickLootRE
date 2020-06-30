#pragma once

class ContainerHandler :
	public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
	ContainerHandler() = delete;
	ContainerHandler(const ContainerHandler&) = default;
	ContainerHandler(ContainerHandler&&) = default;

	inline ContainerHandler(RE::TESObjectREFRPtr a_container) :
		_container(std::move(a_container))
	{
		assert(_container != nullptr);
		Register();
	}

	inline ~ContainerHandler() { Unregister(); }

	ContainerHandler& operator=(const ContainerHandler&) = default;
	ContainerHandler& operator=(ContainerHandler&&) = default;

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

	RE::TESObjectREFRPtr _container;
};
