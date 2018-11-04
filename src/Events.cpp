#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameMenus.h"  // UIManager
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <map>  // map

#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu

#include "RE/BaseExtraList.h"  // RE::BaseExtraList
#include "RE/InventoryEntryData.h"  // RE::InventoryEntryData
#include "RE/PlayerCharacter.h"  // RE::PlayerCharacter
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
		RE::InventoryEntryData* entryData = reinterpret_cast<RE::InventoryEntryData*>(a_entryData);
		if (entryData) {
			auto it = defaultMap.find(entryData->type);
			if (it != defaultMap.end()) {
				SInt32 count = it->second + entryData->countDelta;
				if (count > 0) {
					g_invList.add(entryData, count);
				} else {
					defaultMap.erase(entryData->type);
				}
			} else if (entryData->countDelta > 0) {
				g_invList.add(entryData);
			}
		}
		return true;
	}


	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		static UIManager*			uiManager	= UIManager::GetSingleton();
		static RE::PlayerCharacter*	player		= reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);


		// If player is not looking at anything
		if (!a_event || !a_event->crosshairRef) {
			if (LootMenu::IsOpen()) {
				CALL_MEMBER_FN(uiManager, AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
				LootMenu::ClearContainerRef();
			}
			return kEvent_Continue;
		}

		// If player went from container -> container
		RE::TESObjectREFR* ref = reinterpret_cast<RE::TESObjectREFR*>(a_event->crosshairRef);
		if (LootMenu::IsOpen() && (LootMenu::GetContainerRef() != ref)) {
			CALL_MEMBER_FN(uiManager, AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
			LootMenu::ClearContainerRef();
		}

		// If player is looking at a container
		if (LootMenu::CanOpen(ref, player->IsSneaking())) {
			LootMenu::SetContainerRef(ref);
			TESContainer* container = ref->GetContainer();
			g_invList.clear();
			defaultMap.clear();
			ItemData::setContainer(ref);
			getInventoryList(&ref->extraData, container);
			g_invList.sort();
			CALL_MEMBER_FN(uiManager, AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
			CALL_MEMBER_FN(uiManager, AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Open, 0);
		}
		return kEvent_Continue;
	}


	EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher)
	{
		if (LootMenu::IsOpen() && a_event) {
			RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
			if (a_event->fromFormId == ref->formID || a_event->toFormId == ref->formID) {
				TESContainer* container = ref->GetContainer();
				if (container) {
					g_invList.clear();
					defaultMap.clear();
					ItemData::setContainer(ref);
					getInventoryList(&ref->extraData, container);
					g_invList.sort();
					LootMenu::Register(LootMenu::kScaleform_OpenContainer);
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
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}
