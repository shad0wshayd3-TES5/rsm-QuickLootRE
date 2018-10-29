#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // InventoryEntryData, ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameMenus.h"  // UIManager
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <map>  // map

#include "Hooks.h"  // startActivation()
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu

#include "RE/BaseExtraList.h"  // RE::BaseExtraList
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


namespace QuickLootRE
{
	bool TESContainerVisitor::Accept(TESContainer::Entry* a_entry)
	{
		defaultMap[a_entry->form] = a_entry->count;
		return true;
	}


	bool EntryDataListVisitor::Accept(InventoryEntryData* a_entryData)
	{
		if (a_entryData) {
			auto it = defaultMap.find(a_entryData->type);
			if (it != defaultMap.end()) {
				SInt32 count = it->second + a_entryData->countDelta;
				if (count > 0) {
					g_invList.add(a_entryData, count);
				} else {
					defaultMap.erase(a_entryData->type);
				}
			} else if (a_entryData->countDelta > 0) {
				g_invList.add(a_entryData);
			}
		}
		return true;
	}


	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		if (a_event && a_event->crosshairRef) {
			RE::TESObjectREFR* ref = reinterpret_cast<RE::TESObjectREFR*>(a_event->crosshairRef);
			if (LootMenu::CanOpen(ref)) {
				LootMenu::SetContainerRef(ref);
				TESContainer* container = ref->GetContainer();
				g_invList.clear();
				defaultMap.clear();
				getInventoryList(&ref->extraData, container);
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Open, 0);
			}
		} else if (LootMenu::GetSingleton()) {
			CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
			LootMenu::ClearContainerRef();
		}
		return kEvent_Continue;
	}


	EventResult MenuOpenCloseEventHandler::ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher)
	{
		return kEvent_Continue;
		// TODO
	}


	EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher)
	{
		LootMenu* singleton = LootMenu::GetSingleton();
		if (singleton && a_event) {
			RE::TESObjectREFR* ref = singleton->GetContainerRef();
			if (a_event->fromFormId == ref->baseForm->formID || a_event->toFormId == ref->baseForm->formID) {
				TESContainer* container = ref->GetContainer();
				if (container) {
					g_invList.clear();
					getInventoryList(&ref->extraData, container);
					LootMenu::GetSingleton()->OpenContainer();
				}
			}
		}
		return kEvent_Continue;
	}


	void getInventoryList(RE::BaseExtraList* a_xList, TESContainer* a_container)
	{
		// Default container
		TESContainerVisitor containerOp;
		a_container->Visit(containerOp);

		// Extra container changes
		ExtraContainerChanges* xContainerChanges = static_cast<ExtraContainerChanges*>(a_xList->GetByType(kExtraData_ContainerChanges));
		EntryDataListVisitor entryDataListOp;
		if (xContainerChanges && xContainerChanges->data && xContainerChanges->data->objList) {
			xContainerChanges->data->objList->Visit(entryDataListOp);
		}

		// Add remaining default items
		for (auto& it : defaultMap) {
			g_invList.add(it.first, it.second);
		}
	}


	CrosshairRefEventHandler g_crosshairRefEventHandler;
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	InputEventHandler g_inputEventHandler;
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}