#include "Events.h"

#include "Loot.h"

namespace Events
{
	auto CrosshairRefManager::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto crosshairRef =
			a_event->crosshairRef ?
				a_event->crosshairRef->CreateRefHandle() :
				RE::ObjectRefHandle();
		if (_cachedRef == crosshairRef) {
			return EventResult::kContinue;
		}

		_cachedRef = crosshairRef;
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
