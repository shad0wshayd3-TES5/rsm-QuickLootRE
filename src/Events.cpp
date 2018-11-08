#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameMenus.h"  // UIStringHolder
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameMenus.h"  // UIManager
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <utility>  // pair
#include <map>  // map

#include "Delegates.h"  // DelayedUpdater
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/IMenu.h"  // IMenu
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/MenuManager.h"  // MenuManager
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESObjectREFR.h"  // TESObjectREFR


namespace QuickLootRE
{
	bool TESContainerVisitor::Accept(TESContainer::Entry* a_entry)
	{
		defaultMap.emplace(a_entry->form->formID, std::pair<TESForm*, Count>(a_entry->form, a_entry->count));
		return true;
	}


	bool EntryDataListVisitor::Accept(InventoryEntryData* a_entryData)
	{
		RE::InventoryEntryData* entryData = reinterpret_cast<RE::InventoryEntryData*>(a_entryData);
		if (entryData) {
			auto it = defaultMap.find(entryData->type->formID);
			if (it != defaultMap.end()) {
				SInt32 count = it->second.second + entryData->countDelta;
				if (count > 0) {
					g_invList.add(entryData, count);
				}
				defaultMap.erase(entryData->type->formID);
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
				LootMenu::Close();
				LootMenu::ClearContainerRef();
			}
			return kEvent_Continue;
		}

		// If player went from container -> container
		RE::TESObjectREFR* ref = reinterpret_cast<RE::TESObjectREFR*>(a_event->crosshairRef);
		if (LootMenu::IsOpen() && (LootMenu::GetContainerRef() != ref)) {
			LootMenu::Close();
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
			LootMenu::Close();
			LootMenu::Open();
		}
		return kEvent_Continue;
	}


	EventResult MenuOpenCloseEventHandler::ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher)
	{
		static UIStringHolder*	strHolder	= UIStringHolder::GetSingleton();
		static RE::MenuManager*	mm			= RE::MenuManager::GetSingleton();

		if (a_event && LootMenu::IsOpen()) {
			LootMenu* loot = LootMenu::GetSingleton();
			if (a_event->opening) {
				RE::IMenu* menu = mm->GetMenu(&a_event->menuName);
				if (menu) {
					if (menu->StopsCrosshairUpdates() && a_event->menuName != strHolder->tweenMenu) {
						LootMenu::Close();
					} else if (menu->PausesGame()) {
						LootMenu::SetVisible(false);
					}
				}
			} else {
				if (mm->GameIsPaused() && !LootMenu::IsVisible()) {
					LootMenu::SetVisible(true);
					LootMenu::Register(LootMenu::kScaleform_OpenContainer);
				}
			}
		}
		return kEvent_Continue;
	}


	EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher)
	{
		if (a_event && LootMenu::IsVisible() && !LootMenu::InTakeAllMode()) {
			RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
			if (a_event->fromFormId == ref->formID || a_event->toFormId == ref->formID) {
				TESContainer* container = ref->GetContainer();
				if (container) {
					DelayedUpdater::Register();  // These events are fired before the container is updated, so we need to wait a bit
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
			g_invList.add(it.second.first, it.second.second);
		}
	}


	CrosshairRefEventHandler g_crosshairRefEventHandler;
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}
