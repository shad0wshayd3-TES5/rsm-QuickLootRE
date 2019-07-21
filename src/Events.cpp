#include "Events.h"

#include "Delegates.h"
#include "InventoryList.h"
#include "LootMenu.h"
#include "Settings.h"
#include "Utility.h"

#include "SKSE/API.h"
#include "RE/Skyrim.h"


namespace Events
{
	CrosshairRefEventHandler* CrosshairRefEventHandler::GetSingleton()
	{
		static CrosshairRefEventHandler singleton;
		return &singleton;
	}


	auto CrosshairRefEventHandler::ReceiveEvent(SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_dispatcher)
		-> EventResult
	{
		// If player is not looking at anything
		auto loot = LootMenu::GetSingleton();
		if (!a_event->crosshairRef) {
			if (loot->IsOpen()) {
				loot->Close();
				loot->ClearContainerRef();
			}
			return EventResult::kContinue;
		}

		// If player went from container -> container
		RE::TESObjectREFR* ref = a_event->crosshairRef.get();
		if (loot->IsOpen() && loot->GetContainerRef() != ref) {
			loot->Close();
			loot->ClearContainerRef();
		}

		// If player is looking at a container
		auto player = RE::PlayerCharacter::GetSingleton();
		if (ref = loot->CanOpen(ref, player->IsSneaking())) {
			loot->SetContainerRef(ref);
			loot->ParseInventory();
			loot->Open();
		}

		return EventResult::kContinue;
	}


	InputEventHandler* InputEventHandler::GetSingleton()
	{
		static InputEventHandler singleton;
		return &singleton;
	}


	auto InputEventHandler::ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
		-> EventResult
	{
		using EventType = RE::InputEvent::EventType;
		using DeviceType = RE::DeviceType;
		using Message = RE::UIMessage::Message;

		if (!a_event || !*a_event) {
			return EventResult::kContinue;
		}
		auto event = *a_event;

		auto loot = LootMenu::GetSingleton();
		if (loot->IsOpen()) {
			if (event->eventType == EventType::kButton && event->deviceType == DeviceType::kKeyboard) {
				auto button = static_cast<RE::ButtonEvent*>(event);

				auto inputStrHolder = RE::InputStringHolder::GetSingleton();
				if (button->GetControlID() == inputStrHolder->nextFocus) {  // Tab
					auto mm = RE::MenuManager::GetSingleton();
					auto uiStrHolder = RE::UIStringHolder::GetSingleton();
					auto uiManager = RE::UIManager::GetSingleton();

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


	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
	{
		static MenuOpenCloseEventHandler singleton;
		return &singleton;
	}


	auto MenuOpenCloseEventHandler::ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		auto loot = LootMenu::GetSingleton();
		if (!loot->IsOpen()) {
			return EventResult::kContinue;
		}

		auto mm = RE::MenuManager::GetSingleton();
		if (a_event->isOpening) {
			auto menu = mm->GetMenu(a_event->menuName);
			if (menu) {
				auto uiStrHolder = RE::UIStringHolder::GetSingleton();
				auto& menuName = a_event->menuName;
				if (menuName == uiStrHolder->dialogueMenu || menuName == uiStrHolder->messageBoxMenu) {
					loot->Close();
				} else if ((menu->StopsCrosshairUpdates() && menuName != uiStrHolder->tweenMenu) || menu->PausesGame()) {
					loot->SetVisible(false);
				}
			}
		} else {
			if (!loot->IsVisible() && !mm->GameIsPaused()) {
				loot->SetVisible(true);
				loot->ParseInventory();
				Dispatch<OpenContainerDelegate>();
			}
		}

		return EventResult::kContinue;
	}


	TESCombatEventHandler* TESCombatEventHandler::GetSingleton()
	{
		static TESCombatEventHandler singleton;
		return &singleton;
	}


	auto TESCombatEventHandler::ReceiveEvent(RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource)
		-> EventResult
	{
		auto loot = LootMenu::GetSingleton();
		if (!loot->IsOpen()) {
			return EventResult::kContinue;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if ((a_event->source && a_event->source->IsPlayerRef()) || (a_event->target && a_event->target->IsPlayerRef())) {
			if (Settings::disableInCombat || IsValidPickPocketTarget(loot->GetContainerRef(), player->IsSneaking())) {
				loot->Close();
				loot->SkipNextInput();
			}
		}

		return EventResult::kContinue;
	}


	TESContainerChangedEventHandler* TESContainerChangedEventHandler::GetSingleton()
	{
		static TESContainerChangedEventHandler singleton;
		return &singleton;
	}


	auto TESContainerChangedEventHandler::ReceiveEvent(RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
		-> EventResult
	{
		auto loot = LootMenu::GetSingleton();
		if (!loot->IsVisible() || loot->CanProcessInventoryChanges()) {
			return EventResult::kContinue;
		}

		auto ref = loot->GetContainerRef();
		if (!ref) {
			return EventResult::kContinue;
		}

		if (a_event->from == ref->formID || a_event->to == ref->formID) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask(new DelayedUpdater());	// This event is fired before the item is removed, so we have to wait a bit
		}

		return EventResult::kContinue;
	}
}
