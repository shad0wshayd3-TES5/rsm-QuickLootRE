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


	auto CrosshairRefEventHandler::ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_dispatcher)
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


	auto InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
		-> EventResult
	{
		using EventType = RE::INPUT_EVENT_TYPE;
		using DeviceType = RE::INPUT_DEVICE;
		using Message = RE::UIMessage::Message;

		if (!a_event || !*a_event) {
			return EventResult::kContinue;
		}
		auto event = *a_event;

		auto loot = LootMenu::GetSingleton();
		if (loot->IsOpen()) {
			if (event->eventType == EventType::kButton && event->device == DeviceType::kKeyboard) {
				auto button = static_cast<RE::ButtonEvent*>(event);

				auto events = RE::UserEvents::GetSingleton();
				if (button->QUserEvent() == events->nextFocus) {  // Tab
					auto ui = RE::UI::GetSingleton();
					auto intfcStr = RE::InterfaceStrings::GetSingleton();
					auto msgQ = RE::UIMessageQueue::GetSingleton();

					if (ui->GetMovieView(intfcStr->inventoryMenu)) {
						msgQ->AddMessage(intfcStr->inventoryMenu, Message::kClose, 0);
					} else if (ui->GetMovieView(intfcStr->statsMenu) && !ui->GetMovieView(intfcStr->levelUpMenu)) {
						msgQ->AddMessage(intfcStr->statsMenu, Message::kClose, 0);
					} else if (ui->GetMovieView(intfcStr->magicMenu)) {
						msgQ->AddMessage(intfcStr->magicMenu, Message::kClose, 0);
					} else if (ui->GetMovieView(intfcStr->mapMenu)) {
						msgQ->AddMessage(intfcStr->mapMenu, Message::kClose, 0);
					} else if (ui->GetMovieView(intfcStr->containerMenu)) {
						msgQ->AddMessage(intfcStr->containerMenu, Message::kClose, 0);
					} else if (ui->GetMovieView(intfcStr->journalMenu)) {
						msgQ->AddMessage(intfcStr->journalMenu, Message::kClose, 0);
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


	auto MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		auto loot = LootMenu::GetSingleton();
		if (!loot->IsOpen()) {
			return EventResult::kContinue;
		}

		auto ui = RE::UI::GetSingleton();
		if (a_event->opening) {
			auto menu = ui->GetMenu(a_event->menuName);
			if (menu) {
				auto intfcStr = RE::InterfaceStrings::GetSingleton();
				auto& menuName = a_event->menuName;
				if (menuName == intfcStr->dialogueMenu || menuName == intfcStr->messageBoxMenu) {
					loot->Close();
				} else if ((menu->DontHideCursorWhenTopmost() && menuName != intfcStr->tweenMenu) || menu->PausesGame()) {
					loot->SetVisible(false);
				}
			}
		} else {
			if (!loot->IsVisible() && !ui->GameIsPaused()) {
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


	auto TESCombatEventHandler::ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource)
		-> EventResult
	{
		auto loot = LootMenu::GetSingleton();
		if (!loot->IsOpen()) {
			return EventResult::kContinue;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if ((a_event->actor && a_event->actor->IsPlayerRef()) || (a_event->targetActor && a_event->targetActor->IsPlayerRef())) {
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


	auto TESContainerChangedEventHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
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

		if (a_event->oldContainer == ref->formID || a_event->newContainer == ref->formID) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask(new DelayedUpdater());	// This event is fired before the item is removed, so we have to wait a bit
		}

		return EventResult::kContinue;
	}
}
