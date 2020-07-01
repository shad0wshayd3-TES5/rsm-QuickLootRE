#include "Events.h"

#include "Loot.h"

namespace Events
{
	auto CrosshairRefManager::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
		-> EventResult
	{
		if (!a_event || _cachedRef == a_event->crosshairRef) {
			return EventResult::kContinue;
		}

		_cachedRef = a_event->crosshairRef;
		auto loot = Loot::GetSingleton();
		if (CanOpen()) {
			loot->SetContainer(_cachedRef);
			loot->RefreshInventory();
		} else {
			loot->Close();
		}

		return EventResult::kContinue;
	}
}
