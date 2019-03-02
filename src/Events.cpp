#include "Events.h"

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include "Delegates.h"  // DelayedUpdater
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings
#include "Utility.h"  // IsValidPickPocketTarget

#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/IMenu.h"  // IMenu
#include "RE/InputEvent.h"  // InputEvent
#include "RE/InputStringHolder.h"  // InputStringHolder
#include "RE/MenuManager.h"  // MenuManager
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/UIManager.h"  // UIManager
#include "RE/UIStringHolder.h"  // UIStringHolder


namespace Events
{
	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		if (!a_event) {
			return kEvent_Continue;
		}

		// If player is not looking at anything
		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event->crosshairRef) {
			if (loot->IsOpen()) {
				loot->Close();
				loot->ClearContainerRef();
			}
			return kEvent_Continue;
		}

		// If player went from container -> container
		RE::TESObjectREFR* ref = reinterpret_cast<RE::TESObjectREFR*>(a_event->crosshairRef);
		if (loot->IsOpen() && loot->GetContainerRef() != ref) {
			loot->Close();
			loot->ClearContainerRef();
		}

		// If player is looking at a container
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (ref = loot->CanOpen(ref, player->IsSneaking())) {
			loot->SetContainerRef(ref);
			loot->ParseInventory();
			loot->Open();
		}

		return kEvent_Continue;
	}


	CrosshairRefEventHandler* CrosshairRefEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new CrosshairRefEventHandler();
		}
		return _singleton;
	}


	void CrosshairRefEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	CrosshairRefEventHandler::CrosshairRefEventHandler()
	{}


	CrosshairRefEventHandler::~CrosshairRefEventHandler()
	{}


	CrosshairRefEventHandler* CrosshairRefEventHandler::_singleton = 0;


	RE::EventResult InputEventHandler::ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
	{
		using RE::EventResult;
		using EventType = RE::InputEvent::EventType;
		using DeviceType = RE::DeviceType;
		using Keyboard = RE::BSWin32KeyboardDevice::Keyboard;
		using Message = RE::UIMessage::Message;

		if (!a_event || !*a_event) {
			return EventResult::kContinue;
		}

		if (LootMenu::GetSingleton()->IsOpen()) {
			if ((*a_event)->eventType == EventType::kButton && (*a_event)->deviceType == DeviceType::kKeyboard) {
				RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(*a_event);

				if (button->GetControlID() == RE::InputStringHolder::GetSingleton()->nextFocus) {  // Tab
					RE::MenuManager* mm = RE::MenuManager::GetSingleton();
					RE::UIStringHolder* uiStrHolder = RE::UIStringHolder::GetSingleton();
					RE::UIManager* uiManager = RE::UIManager::GetSingleton();

					if (mm->GetMovieView(uiStrHolder->inventoryMenu)) {
						uiManager->AddMessage(uiStrHolder->inventoryMenu, Message::kClose, 0);
					} else if (mm->GetMovieView(uiStrHolder->statsMenu) && !mm->GetMovieView(uiStrHolder->levelUpMenu)) {
						uiManager->AddMessage(uiStrHolder->statsMenu, Message::kClose, 0);
					} else if (mm->GetMovieView(uiStrHolder->magicMenu)) {
						uiManager->AddMessage(uiStrHolder->magicMenu, Message::kClose, 0);
					} else if (mm->GetMovieView(uiStrHolder->mapMenu)) {
						uiManager->AddMessage(uiStrHolder->mapMenu, Message::kClose, 0);
					} else if (mm->GetMovieView(uiStrHolder->containerMenu)) {
						uiManager->AddMessage(uiStrHolder->containerMenu, Message::kClose, 0);
					} else if (mm->GetMovieView(uiStrHolder->journalMenu)) {
						uiManager->AddMessage(uiStrHolder->journalMenu, Message::kClose, 0);
					}
				}
			}
		}
		return EventResult::kContinue;
	}


	InputEventHandler* InputEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new InputEventHandler();
		}
		return _singleton;
	}


	void InputEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	InputEventHandler::InputEventHandler()
	{}


	InputEventHandler::~InputEventHandler()
	{}


	InputEventHandler* InputEventHandler::_singleton = 0;


	RE::EventResult MenuOpenCloseEventHandler::ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		using RE::EventResult;

		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event || !loot || !loot->IsOpen()) {
			return EventResult::kContinue;
		}

		RE::BSFixedString menuName = a_event->menuName;
		RE::MenuManager* mm = RE::MenuManager::GetSingleton();

		if (a_event->isOpening) {
			RE::IMenu* menu = mm->GetMenu(a_event->menuName);

			if (menu) {
				RE::UIStringHolder* strHolder = RE::UIStringHolder::GetSingleton();

				if (menuName == strHolder->dialogueMenu || menuName == strHolder->messageBoxMenu) {
					loot->Close();
				} else if ((menu->StopsCrosshairUpdates() && menuName != strHolder->tweenMenu) || menu->PausesGame()) {
					loot->SetVisible(false);
				}
			}
		} else {
			if (!loot->IsVisible() && !mm->GameIsPaused()) {
				loot->SetVisible(true);
				loot->ParseInventory();
				loot->Register(LootMenu::Scaleform::kOpenContainer);
			}
		}

		return EventResult::kContinue;
	}


	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new MenuOpenCloseEventHandler();
		}
		return _singleton;
	}


	void MenuOpenCloseEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	MenuOpenCloseEventHandler::MenuOpenCloseEventHandler()
	{}


	MenuOpenCloseEventHandler::~MenuOpenCloseEventHandler()
	{}


	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::_singleton = 0;


	RE::EventResult TESCombatEventHandler::ReceiveEvent(RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource)
	{
		using RE::EventResult;

		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event || !loot->IsOpen()) {
			return EventResult::kContinue;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if ((a_event->source && a_event->source->IsPlayerRef()) || (a_event->target && a_event->target->IsPlayerRef())) {
			if (Settings::disableInCombat || IsValidPickPocketTarget(loot->GetContainerRef(), player->IsSneaking())) {
				loot->Close();
				loot->SkipNextInput();
			}
		}

		return EventResult::kContinue;
	}


	TESCombatEventHandler* TESCombatEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new TESCombatEventHandler();
		}
		return _singleton;
	}


	void TESCombatEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	TESCombatEventHandler::TESCombatEventHandler()
	{}


	TESCombatEventHandler::~TESCombatEventHandler()
	{}


	TESCombatEventHandler* TESCombatEventHandler::_singleton = 0;


	RE::EventResult TESContainerChangedEventHandler::ReceiveEvent(RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		using RE::EventResult;

		LootMenu* loot = LootMenu::GetSingleton();
		if (!a_event || !loot->IsVisible() || loot->CanProcessInventoryChanges()) {
			return EventResult::kContinue;
		}

		RE::TESObjectREFR* ref = loot->GetContainerRef();
		if (!ref) {
			return EventResult::kContinue;
		}

		if (a_event->fromFormID == ref->formID || a_event->toFormID == ref->formID) {
			DelayedUpdater::Register();  // This event is fired before the item is removed, so we have to wait a bit
		}

		return EventResult::kContinue;
	}


	TESContainerChangedEventHandler* TESContainerChangedEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new TESContainerChangedEventHandler();
		}
		return _singleton;
	}


	void TESContainerChangedEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	TESContainerChangedEventHandler::TESContainerChangedEventHandler()
	{}


	TESContainerChangedEventHandler::~TESContainerChangedEventHandler()
	{}


	TESContainerChangedEventHandler* TESContainerChangedEventHandler::_singleton = 0;
}
