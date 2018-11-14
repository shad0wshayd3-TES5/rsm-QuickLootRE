#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameInput.h"  // InputEvent
#include "skse64/GameMenus.h"  // UIStringHolder, UIManager
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <utility>  // pair
#include <map>  // map

#include "Delegates.h"  // DelayedUpdater
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/IMenu.h"  // IMenu
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/MenuManager.h"  // MenuManager
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/UIManager.h"  // UIManager


namespace QuickLootRE
{
	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		static UIManager*			uiManager = UIManager::GetSingleton();
		static RE::PlayerCharacter*	player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);


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
			RE::TESObjectREFR* containerRef = LootMenu::GetContainerRef();
			g_invList.parseInventory(&containerRef->extraData, containerRef);
			LootMenu::Close();
			LootMenu::Open();
		}

		return kEvent_Continue;
	}


	EventResult InputEventHandler::ReceiveEvent(InputEvent** a_event, InputEventDispatcher* a_dispatcher)
	{
		typedef RE::BSWin32KeyboardDevice::Keyboard Keyboard;

		static UIStringHolder*		uiStrHolder		= UIStringHolder::GetSingleton();
		static RE::MenuManager*		mm				= RE::MenuManager::GetSingleton();
		static RE::UIManager*		uiManager		= RE::UIManager::GetSingleton();
		static InputStringHolder*	inputStrHolder	= InputStringHolder::GetSingleton();

		if (!a_event || !*a_event) {
			return kEvent_Continue;
		}

		if (LootMenu::IsOpen()) {
			if ((*a_event)->eventType == InputEvent::kEventType_Button && (*a_event)->deviceType == kDeviceType_Keyboard) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(*a_event);
				if (*button->GetControlID() == inputStrHolder->nextFocus) {  // Tab
					_DMESSAGE("[DEBUG] 0x%p", mm);
					if (mm->GetMovieView(&uiStrHolder->inventoryMenu)) {
						uiManager->AddMessage(uiStrHolder->inventoryMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(&uiStrHolder->statsMenu) && !mm->GetMovieView(&uiStrHolder->levelUpMenu)) {
						uiManager->AddMessage(uiStrHolder->statsMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(&uiStrHolder->magicMenu)) {
						uiManager->AddMessage(uiStrHolder->magicMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(&uiStrHolder->mapMenu)) {
						uiManager->AddMessage(uiStrHolder->mapMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(&uiStrHolder->containerMenu)) {
						uiManager->AddMessage(uiStrHolder->containerMenu, UIMessage::kMessage_Close, 0);
					} else if (mm->GetMovieView(&uiStrHolder->journalMenu)) {
						uiManager->AddMessage(uiStrHolder->journalMenu, UIMessage::kMessage_Close, 0);
					}
				}
			}
		}
		return kEvent_Continue;
	}


	EventResult MenuOpenCloseEventHandler::ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher)
	{
		static UIStringHolder*	strHolder = UIStringHolder::GetSingleton();
		static RE::MenuManager*	mm = RE::MenuManager::GetSingleton();

		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event || !loot || !LootMenu::IsOpen()) {
			return kEvent_Continue;
		}

		BSFixedString menuName = a_event->menuName;
		if (a_event->opening) {
			RE::IMenu* menu = mm->GetMenu(&a_event->menuName);
			if (menu) {
				if (menuName == strHolder->dialogueMenu) {
					LootMenu::Close();
				} else if ((menu->StopsCrosshairUpdates() && menuName != strHolder->tweenMenu) || menu->PausesGame()) {
					LootMenu::SetVisible(false);
				}
			}
		} else {
			if (!LootMenu::IsVisible() && (!mm->GameIsPaused() || menuName == strHolder->dialogueMenu)) {
				LootMenu::SetVisible(true);
				LootMenu::Register(LootMenu::kScaleform_OpenContainer);
			}
		}

		return kEvent_Continue;
	}


	EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher)
	{
		if (!a_event || !LootMenu::IsVisible() || LootMenu::InTakeAllMode()) {
			return kEvent_Continue;
		}

		RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
		if (a_event->fromFormId == ref->formID || a_event->toFormId == ref->formID) {
			DelayedUpdater::Register();  // This event is fired before the item is removed, so we have to wait a bit
		}

		return kEvent_Continue;
	}


	CrosshairRefEventHandler g_crosshairRefEventHandler;
	InputEventHandler g_inputEventHandler;
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}
