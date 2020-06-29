#include "Events.h"

#include "Loot.h"

namespace Events
{
	auto CrosshairRefHandler::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
		-> EventResult
	{
		if (!a_event || _cachedRef == a_event->crosshairRef) {
			return EventResult::kContinue;
		}

		_cachedRef = a_event->crosshairRef;
		auto loot = Loot::GetSingleton();
		if (CanOpen()) {
			loot->SetContainer(_cachedRef);
		} else {
			loot->Close();
		}

		return EventResult::kContinue;
	}
}
