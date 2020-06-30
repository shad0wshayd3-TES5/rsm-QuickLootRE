#include "ContainerHandler.h"

#include "Loot.h"

auto ContainerHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*)
	-> EventResult
{
	if (a_event &&
			a_event->oldContainer == _container->GetFormID() ||
		a_event->newContainer == _container->GetFormID()) {
		auto loot = Loot::GetSingleton();
		loot->RefreshInventory();
	}

	return EventResult::kContinue;
}
