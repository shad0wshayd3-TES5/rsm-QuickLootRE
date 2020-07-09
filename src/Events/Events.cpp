#include "Events.h"

#include "Loot.h"

namespace Events
{
	auto CrosshairRefManager::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
		-> EventResult
	{
		auto crosshairRef =
			a_event && a_event->crosshairRef ?
				a_event->crosshairRef->CreateRefHandle() :
				RE::ObjectRefHandle();
		if (_cachedRef == crosshairRef) {
			return EventResult::kContinue;
		}

		_cachedRef = crosshairRef;
		_cachedAshPile.reset();
		auto loot = Loot::GetSingleton();
		if (CanOpen(a_event->crosshairRef)) {
			loot->SetContainer(_cachedAshPile ? _cachedAshPile : _cachedRef);
			loot->RefreshInventory();
		} else {
			loot->Close();
		}

		return EventResult::kContinue;
	}
}
