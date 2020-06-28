#include "Events.h"

#include "Loot.h"

namespace Events
{
	CrosshairRefHandler* CrosshairRefHandler::GetSingleton()
	{
		static CrosshairRefHandler singleton;
		return std::addressof(singleton);
	}

	void CrosshairRefHandler::Register()
	{
		auto source = SKSE::GetCrosshairRefEventSource();
		if (source) {
			source->AddEventSink(GetSingleton());
			_MESSAGE("Registered %s", typeid(CrosshairRefHandler).name());
		}
	}

	auto CrosshairRefHandler::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, [[maybe_unused]] RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || _cachedRef == a_event->crosshairRef) {
			return EventResult::kContinue;
		}

		_cachedRef = a_event->crosshairRef;
		auto loot = Loot::GetSingleton();
		if (_cachedRef && _cachedRef->GetContainer()) {
			loot->SetContainer(_cachedRef);
		} else {
			loot->Close();
		}

		return EventResult::kContinue;
	}

	void Register()
	{
		CrosshairRefHandler::Register();
		_MESSAGE("Registered all event handlers");
	}
}
