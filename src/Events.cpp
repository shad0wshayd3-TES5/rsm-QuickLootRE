#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameMenus.h"  // UIManager
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <utility>  // pair
#include <map>  // map

#include "Delegates.h"  // DelayedUpdater
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/IMenu.h"  // IMenu
#include "RE/InputEvent.h"  // InputEvent
#include "RE/InputStringHolder.h"  // InputStringHolder
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/MenuManager.h"  // MenuManager
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/UIManager.h"  // UIManagerUIStringHolder
#include "RE/UIStringHolder.h"  // UIStringHolder


namespace QuickLootRE
{
	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		if (!a_event) {
			return kEvent_Continue;
		}

		// If player is not looking at anything
		if (!a_event->crosshairRef) {
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
		if (LootMenu::CanOpen(ref, RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
			g_invList.parseInventory(LootMenu::GetContainerRef());
			LootMenu::Open();
		}

		return kEvent_Continue;
	}


	RE::EventResult InputEventHandler::ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_dispatcher)
	{
		using RE::EventResult;

		typedef RE::InputEvent::EventType			EventType;
		typedef RE::InputEvent::DeviceType			DeviceType;
		typedef RE::BSWin32KeyboardDevice::Keyboard	Keyboard;

		if (!a_event || !*a_event) {
			return EventResult::kEvent_Continue;
		}

		if (LootMenu::IsOpen()) {
			if ((*a_event)->eventType == EventType::kButton && (*a_event)->deviceType == DeviceType::kKeyboard) {

				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(*a_event);

				if (button->GetControlID() == RE::InputStringHolder::GetSingleton()->nextFocus) {  // Tab

					RE::MenuManager* mm = RE::MenuManager::GetSingleton();
					RE::UIStringHolder* uiStrHolder = RE::UIStringHolder::GetSingleton();
					RE::UIManager* uiManager = RE::UIManager::GetSingleton();

					if (mm->GetMovieView(uiStrHolder->inventoryMenu)) {
						uiManager->AddMessage(uiStrHolder->inventoryMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(uiStrHolder->statsMenu) && !mm->GetMovieView(uiStrHolder->levelUpMenu)) {
						uiManager->AddMessage(uiStrHolder->statsMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(uiStrHolder->magicMenu)) {
						uiManager->AddMessage(uiStrHolder->magicMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(uiStrHolder->mapMenu)) {
						uiManager->AddMessage(uiStrHolder->mapMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(uiStrHolder->containerMenu)) {
						uiManager->AddMessage(uiStrHolder->containerMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(uiStrHolder->journalMenu)) {
						uiManager->AddMessage(uiStrHolder->journalMenu, UIMessage::kMessage_Close, 0);
					}
				}
			}
		}
		return EventResult::kEvent_Continue;
	}


	RE::EventResult MenuOpenCloseEventHandler::ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_dispatcher)
	{
		using RE::EventResult;

		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event || !loot || !LootMenu::IsOpen()) {
			return EventResult::kEvent_Continue;
		}

		RE::BSFixedString menuName = a_event->menuName;
		RE::MenuManager* mm = RE::MenuManager::GetSingleton();

		if (a_event->isOpening) {
			RE::IMenu* menu = mm->GetMenu(a_event->menuName);

			if (menu) {
				RE::UIStringHolder* strHolder = RE::UIStringHolder::GetSingleton();

				if (menuName == strHolder->dialogueMenu || menuName == strHolder->messageBoxMenu) {
					LootMenu::Close();
				} else if ((menu->StopsCrosshairUpdates() && menuName != strHolder->tweenMenu) || menu->PausesGame()) {
					LootMenu::SetVisible(false);
				}
			}

		} else {
			if (!LootMenu::IsVisible() && !mm->GameIsPaused()) {
				LootMenu::SetVisible(true);
				g_invList.parseInventory(LootMenu::GetContainerRef());
				LootMenu::Register(LootMenu::Scaleform::kOpenContainer);
			}
		}

		return EventResult::kEvent_Continue;
	}


	RE::EventResult TESCombatEventHandler::ReceiveEvent(TESCombatEvent* a_event, RE::BSTEventSource<TESCombatEvent>* a_dispatcher)
	{
		using RE::EventResult;

		if (!a_event || !LootMenu::IsOpen()) {
			return EventResult::kEvent_Continue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if ((a_event->source && a_event->source->formID == player->formID) || (a_event->target && a_event->target->formID == player->formID)) {
			if (IsValidPickPocketTarget(LootMenu::GetContainerRef(), player->IsSneaking()) || Settings::disableInCombat) {
				LootMenu::Close();
				LootMenu::SkipNextInput();
			}
		}

		return EventResult::kEvent_Continue;
	}


	RE::EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, RE::BSTEventSource<TESContainerChangedEvent>* a_dispatcher)
	{
		using RE::EventResult;

		if (!a_event || !LootMenu::IsVisible() || LootMenu::InTakeAllMode()) {
			return EventResult::kEvent_Continue;
		}

		RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
		if (!ref) {
			return EventResult::kEvent_Continue;
		}

		if (a_event->fromFormId == ref->formID || a_event->toFormId == ref->formID) {
			DelayedUpdater::Register();  // This event is fired before the item is removed, so we have to wait a bit
		}

		return EventResult::kEvent_Continue;
	}


	CrosshairRefEventHandler g_crosshairRefEventHandler;
	InputEventHandler g_inputEventHandler;
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	TESCombatEventHandler g_combatEventHandler;
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}
