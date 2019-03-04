#include "LootMenu.h"

#include "skse64/GameMenus.h"  // UIMessage
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/NiRTTI.h"  // ni_cast

#include <cstdlib>  // abort
#include <queue>  // queue
#include <string>  // string

#include "ActivatePerkEntryVisitor.h"  // ActivatePerkEntryVisitor
#include "Delegates.h"
#include "Forms.h"
#include "Hooks.h"  // SendItemsPickPocketedEvent
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList
#include "Settings.h"  // Settings
#include "Utility.h"  // IsValidPickPocketTarget

#include "RE/Actor.h"  // Actor
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/BSWin32MouseDevice.h"  // BSWin32MouseDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/ChestsLooted.h"  // ChestsLooted
#include "RE/DeviceTypes.h"  // DeviceType
#include "RE/ExtraDataTypes.h"  // ExtraDataType
#include "RE/GFxMovieView.h"  // GFxMovieView
#include "RE/GFxLoader.h"  // GFxLoader
#include "RE/IMenu.h"  // IMenu
#include "RE/InputEvent.h"  // InputEvent
#include "RE/InputManager.h"  // InputManager
#include "RE/InputMappingManager.h"  // InputMappingManager
#include "RE/InputStringHolder.h"  // InputStringHolder
#include "RE/InventoryChanges.h"  // InventoryChanges
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/ItemsPickpocketed.h"  // ItemsPickpocketed
#include "RE/MenuControls.h"  // MenuControls
#include "RE/MenuManager.h"  // MenuManager
#include "RE/Misc.h"  // DebugNotification, SendItemsPickPocketedEvent
#include "RE/NiControllerManager.h"  // NiControllerManager
#include "RE/NiNode.h"  // NiNode
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESBoundObject.h"  // TESBoundObject
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESRace.h"  // TESRace
#include "RE/UIManager.h"  // UIManager
#include "RE/UIMessage.h"  // UIMessage
#include "RE/UIStringHolder.h"  // UIStringHolder


RE::IMenu::Result LootMenu::ProcessMessage(RE::UIMessage* a_message)
{
	using UIMessage = RE::UIMessage::Message;


	if (!view) {
		_FATALERROR("[FATAL ERROR] LootMenu is missing a view! Dependencies were not loaded!\n");
		QueueMessage(Message::kMissingDependencies);
		return Result::kNotProcessed;
	}

	if (!Settings::isApplied) {
		Register(Scaleform::kSetup);
		Register(Scaleform::kSwitchStyle);
	}

	switch (a_message->message) {
	case UIMessage::kOpen:
		OnMenuOpen();
		break;
	case UIMessage::kClose:
		OnMenuClose();
		break;
	}

	return Result::kNotProcessed;
}


void LootMenu::Render()
{
	if (IsOpen()) {
		view->Display();
	}
}


bool LootMenu::CanProcess(RE::InputEvent* a_event)
{
	using DeviceType = RE::DeviceType;
	using EventType = RE::InputEvent::EventType;
	using Gamepad = RE::BSWin32GamepadDevice::Gamepad;
	using Mouse = RE::BSWin32MouseDevice::Mouse;

	if (IsOpen() && a_event->eventType == EventType::kButton) {
		RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);

		RE::BSFixedString controlID = a_event->GetControlID();
		RE::InputStringHolder* strHolder = RE::InputStringHolder::GetSingleton();
		if (controlID == strHolder->sneak) {
			return true;
		}

		switch (a_event->deviceType) {
		case DeviceType::kGamepad:
			{
				Gamepad keyMask = static_cast<Gamepad>(button->keyMask);
				return (keyMask == Gamepad::kUp || keyMask == Gamepad::kDown);
			}
			break;
		case DeviceType::kMouse:
			{
				Mouse keyMask = static_cast<Mouse>(button->keyMask);
				return (keyMask == Mouse::kWheelDown || keyMask == Mouse::kWheelUp);
			}
			break;
		case DeviceType::kKeyboard:
			return (controlID == strHolder->zoomIn || controlID == strHolder->zoomOut);
			break;
		}
	}
	return false;
}


bool LootMenu::ProcessButton(RE::ButtonEvent* a_event)
{
	using DeviceType = RE::DeviceType;
	using Gamepad = RE::BSWin32GamepadDevice::Gamepad;
	using Mouse = RE::BSWin32MouseDevice::Mouse;

	if (!a_event->IsDown()) {
		return true;
	}

	RE::BSFixedString controlID = a_event->GetControlID();
	RE::InputStringHolder* strHolder = RE::InputStringHolder::GetSingleton();
	if (controlID == strHolder->sneak) {
		RE::TESObjectREFR* ref = _containerRef;
		Close();
		SkipNextInput();
		if (CanOpen(ref, !RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
			Open();
		}
		return true;
	}

	switch (a_event->deviceType) {
	case DeviceType::kGamepad:
		_platform = Platform::kOther;
		Register(Scaleform::kSetPlatform);
		Register(Scaleform::kUpdateButtons);
		switch (Gamepad(a_event->keyMask)) {
		case Gamepad::kUp:
			ModSelectedIndex(-1);
			break;
		case Gamepad::kDown:
			ModSelectedIndex(1);
			break;
		}
		break;
	case DeviceType::kMouse:
		_platform = Platform::kPC;
		Register(Scaleform::kSetPlatform);
		Register(Scaleform::kUpdateButtons);
		switch (Mouse(a_event->keyMask)) {
		case Mouse::kWheelUp:
			ModSelectedIndex(-1);
			break;
		case Mouse::kWheelDown:
			ModSelectedIndex(1);
			break;
		}
		break;
	case DeviceType::kKeyboard:
		_platform = Platform::kPC;
		Register(Scaleform::kSetPlatform);
		Register(Scaleform::kUpdateButtons);
		if (controlID == strHolder->zoomIn) {
			ModSelectedIndex(-1);
		} else if (controlID == strHolder->zoomOut) {
			ModSelectedIndex(1);
		}
		break;
	}
	return true;
}


LootMenu* LootMenu::GetSingleton()
{
	if (!_singleton) {
		_singleton = new LootMenu(LootMenu::GetName().c_str());
	}
	return _singleton;
}


void LootMenu::Free()
{
	delete _singleton;
	_singleton = 0;
}


RE::BSFixedString LootMenu::GetName()
{
	static RE::BSFixedString name = "LootMenu";
	return name;
}


bool LootMenu::IsConstructed()
{
	return _singleton && _singleton->view;
}


const char* LootMenu::GetSingleLootMapping()
{
	return _singleLootMapping.c_str();
}


void LootMenu::SetSingleLootMapping(const char* a_singLootMapping)
{
	_singleLootMapping = a_singLootMapping;
}


const char* LootMenu::GetTakeMapping()
{
	return _takeMapping.c_str();
}


void LootMenu::SetTakeMapping(const char* a_takeStr)
{
	_takeMapping = a_takeStr;
}


const char* LootMenu::GetTakeAllMapping()
{
	return _takeAllMapping.c_str();
}


void LootMenu::SetTakeAllMapping(const char* a_takeAllStr)
{
	_takeAllMapping = a_takeAllStr;
}


const char* LootMenu::GetSearchMapping()
{
	return _searchMapping.c_str();
}


void LootMenu::SetSearchMapping(const char* a_searchStr)
{
	_searchMapping = a_searchStr;
}


void LootMenu::QueueMessage(Message a_msg)
{
	switch (a_msg) {
	case Message::kNoInputLoaded:
		_messageQueue.push("$QuickLootRE_NoInputLoaded");
		break;
	case Message::kHookShareMissing:
		_messageQueue.push("$QuickLootRE_HookShareMissing");
		break;
	case Message::kHookShareIncompatible:
		_messageQueue.push("$QuickLootRE_HookShareIncompatible");
		break;
	case Message::kMissingDependencies:
		_messageQueue.push("$QuickLootRE_MissingDependencies");
		ProcessMessageQueue();
		break;
	case Message::kLootMenuToggled:
		{
			static const char* enabled = "$QuickLootRE_LootMenuToggled_Enabled";
			static const char* disabled = "$QuickLootRE_LootMenuToggled_Disabled";
			const char* state = LootMenu::IsEnabled() ? enabled : disabled;
			_messageQueue.push(state);
			ProcessMessageQueue();
		}
		break;
	default:
		_ERROR("[ERROR] Invalid message (%i)", a_msg);
		break;
	}

	if (IsConstructed() && LootMenu::GetSingleton()->IsOpen()) {
		ProcessMessageQueue();
	}
}


SInt32 LootMenu::GetSelectedIndex() const
{
	return _selectedIndex;
}


void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
{
	if (IsOpen()) {
		_selectedIndex += a_indexOffset;
		if (_selectedIndex < 0) {
			_selectedIndex = 0;
		} else if (_selectedIndex > _displaySize - 1) {
			_selectedIndex = _displaySize - 1;
		}
		Register(Scaleform::kSetSelectedIndex);
	}
}


void LootMenu::SetDisplaySize(SInt32 a_size)
{
	_displaySize = a_size;
}


bool LootMenu::ShouldSkipNextInput() const
{
	return _skipInputCount;
}


void LootMenu::SkipNextInput()
{
	++_skipInputCount;
}


void LootMenu::NextInputSkipped()
{
	if (_skipInputCount > 0) {
		--_skipInputCount;
	}
}


RE::TESObjectREFR* LootMenu::GetContainerRef() const
{
	return _containerRef;
}


void LootMenu::ClearContainerRef()
{
	PlayAnimationClose();
	_containerRef = 0;
}


bool LootMenu::IsOpen() const
{
	return _isMenuOpen;
}


bool LootMenu::IsVisible() const
{
	return view->GetVisible();
}


bool LootMenu::CanProcessInventoryChanges() const
{
	return _canProcessInvChanges;
}


bool LootMenu::GetEnabled() const
{
	return _isEnabled;
}


void LootMenu::SetEnabled(bool a_enable)
{
	_isEnabled = a_enable;
	if (!_isEnabled) {
		Close();
	}
}


void LootMenu::ToggleEnabled()
{
	SetEnabled(!_isEnabled);
}


LootMenu::Platform LootMenu::GetPlatform() const
{
	return _platform;
}


const char* LootMenu::GetActiText() const
{
	return _actiText.c_str();
}


void LootMenu::SetActiText(const char* a_actiText)
{
	_actiText = a_actiText;
}


void LootMenu::Open() const
{
	if (_isEnabled) {
		RE::UIManager::GetSingleton()->AddMessage(GetName(), RE::UIMessage::Message::kOpen, 0);
	}
}


void LootMenu::Close() const
{
	RE::UIManager::GetSingleton()->AddMessage(GetName(), RE::UIMessage::Message::kClose, 0);
}


void LootMenu::SetVisible(bool a_visible)
{
	using Context = RE::InputMappingManager::Contexts;

	RE::MenuControls* mc = RE::MenuControls::GetSingleton();
	view->SetVisible(a_visible);
	if (a_visible && !_isRegistered) {
		mc->RegisterHandler(this);
		_isRegistered = true;
	} else if (!a_visible && _isRegistered) {
		mc->RemoveHandler(this);
		_isRegistered = false;
	}
}


void LootMenu::SetContainerRef(RE::TESObjectREFR* a_ref)
{
	_containerRef = a_ref;
}


RE::TESObjectREFR* LootMenu::CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking) const
{
	using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;

	static RE::BSFixedString strAnimationDriven = "bAnimationDriven";

	if (!LootMenu::GetEnabled()) {
		return 0;
	}

	if (!a_ref || !a_ref->baseForm) {
		return 0;
	}

	RE::MenuManager* mm = RE::MenuManager::GetSingleton();
	RE::UIStringHolder* strHolder = RE::UIStringHolder::GetSingleton();
	if (mm->GameIsPaused() || mm->CrosshairIsPaused() || mm->GetMenu(strHolder->dialogueMenu)) {
		return 0;
	}

	RE::InputMappingManager* mappingManager = RE::InputMappingManager::GetSingleton();
	if (!mappingManager->IsMovementControlsEnabled()) {
		return 0;
	}

	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	if (player->GetGrabbedRef() || player->GetActorInFavorState() || player->IsInKillMove()) {
		return 0;
	}

	bool bAnimationDriven;
	if (player->GetAnimationVariableBool(strAnimationDriven, bAnimationDriven) && bAnimationDriven) {
		return 0;
	}

	if (Settings::disableInCombat && player->IsInCombat()) {
		return 0;
	}

	if (Settings::disableTheft && a_ref->IsOffLimits()) {
		return 0;
	}

	if (a_ref->IsActivationBlocked()) {
		return 0;
	}

	RE::TESObjectREFR* containerRef = 0;
	switch (a_ref->baseForm->formType) {
	case RE::FormType::Activator:
		{
			UInt32 refHandle = 0;
			if (a_ref->extraData.GetAshPileRefHandle(refHandle) && refHandle != *g_invalidRefHandle) {
				RE::TESObjectREFRPtr refPtr;
				if (RE::TESObjectREFR::LookupByHandle(refHandle, refPtr)) {
					containerRef = refPtr;
				}
			}
		}
		break;
	case RE::FormType::Container:
		if (!a_ref->IsLocked()) {
			containerRef = a_ref;
		}
		break;
	case RE::FormType::NPC:
		RE::Actor* target = static_cast<RE::Actor*>(a_ref);
		if (Settings::disableForAnimals && target->GetRace()->HasKeyword(ActorTypeAnimal)) {
			return 0;
		} else if (a_ref->IsDead(true) && !target->IsSummoned()) {
			containerRef = a_ref;
		} else if (!Settings::disablePickPocketing && IsValidPickPocketTarget(a_ref, a_isSneaking)) {
			if (!target->IsInCombat()) {
				containerRef = a_ref;
			}
		}
		break;
	}

	if (!containerRef) {
		return 0;
	}

	UInt32 numItems = containerRef->GetNumItems();

	if (Settings::disableIfEmpty && numItems <= 0 && !containerRef->extraData.GetByType(RE::ExtraDataType::kDroppedItemList)) {
		return 0;
	}

	if (Settings::disableForActiOverride && player->CanProcessEntryPointPerkEntry(EntryPoint::kActivate)) {
		ActivatePerkEntryVisitor visitor(player, containerRef);
		player->VisitEntryPointPerkEntries(EntryPoint::kActivate, visitor);
		if (visitor.GetResult()) {
			return 0;
		}
	}

	return containerRef;
}


void LootMenu::Register(Scaleform a_reg) const
{
	if (LootMenu::IsConstructed()) {
		switch (a_reg) {
		case Scaleform::kSetKeyMappings:
			AllocateAndDispatch<SetKeyMappingsUIDelegate>();
			break;
		case Scaleform::kSetPlatform:
			AllocateAndDispatch<SetPlatformUIDelegate>();
			break;
		case Scaleform::kSetSelectedIndex:
			AllocateAndDispatch<SetSelectedIndexUIDelegate>();
			break;
		case Scaleform::kSetup:
			AllocateAndDispatch<SetupUIDelegate>();
			break;
		case Scaleform::kSetContainer:
			AllocateAndDispatch<SetContainerUIDelegate>();
			break;
		case Scaleform::kOpenContainer:
			AllocateAndDispatch<OpenContainerUIDelegate>();
			break;
		case Scaleform::kCloseContainer:
			AllocateAndDispatch<CloseContainerUIDelegate>();
			break;
		case Scaleform::kUpdateButtons:
			AllocateAndDispatch<UpdateButtonsUIDelegate>();
			break;
		case Scaleform::kHideButtons:
			AllocateAndDispatch<HideButtonsUIDelegate>();
			break;
		case Scaleform::kSwitchStyle:
			AllocateAndDispatch<SwitchStyleTaskDelegate>();
			break;
		default:
			_ERROR("[ERROR] Invalid registration (%i)!\n", a_reg);
			break;
		}
	} else {
		_ERROR("[ERROR] The LootMenu has not been constructed!\n");
	}
}


LootMenu::Style LootMenu::GetStyle() const
{
	if (Settings::interfaceStyle == "dialogue") {
		return Style::kDialogue;
	} else {
		if (Settings::interfaceStyle != "default") {
			_ERROR("Invalid style (%s)!", Settings::interfaceStyle.c_str());
			_ERROR("Using default!\n");
		}
		return Style::kDefault;
	}
}


void LootMenu::OnMenuOpen()
{
	using BSGamepadDevice = RE::BSGamepadDevice;
	using BSWin32GamepadDevice = RE::BSWin32GamepadDevice;

	if (!_containerRef) {
		return;
	}

	RE::BSGamepadDevice* gamepadHandle = RE::InputManager::GetSingleton()->GetGamepad();
	RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
	if (gamepad && gamepad->IsEnabled()) {
		_platform = Platform::kOther;
	} else {
		_platform = Platform::kPC;
	}

	_selectedIndex = 0;
	_skipInputCount = 0;
	_isMenuOpen = true;
	Register(Scaleform::kSetKeyMappings);
	Register(Scaleform::kSetPlatform);
	Register(Scaleform::kSetContainer);
	Register(Scaleform::kUpdateButtons);
	if (IsValidPickPocketTarget(_containerRef, RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
		Register(Scaleform::kHideButtons);
	}
	Register(Scaleform::kOpenContainer);
	Register(Scaleform::kSetSelectedIndex);
	SetVisible(true);
	ProcessMessageQueue();
}


void LootMenu::OnMenuClose()
{
	if (IsOpen()) {
		SetVisible(false);
		_isMenuOpen = false;
		Register(Scaleform::kCloseContainer);
		PlayAnimationClose();
	}
}


void LootMenu::TakeItemStack()
{
	if (!IsOpen() || !_containerRef || _displaySize <= 0) {
		return;
	}

	ItemData itemCopy(_invList[_selectedIndex]);

	SInt32 numItems = itemCopy.count();
	if (numItems > 1 && SingleLootEnabled()) {
		numItems = 1;
	}

	if (TakeItem(itemCopy, numItems, true, true)) {
		RE::InventoryChanges* invChanges = RE::PlayerCharacter::GetSingleton()->GetInventoryChanges();
		RE::BaseExtraList* extraList = itemCopy.entryData()->extraList ? itemCopy.entryData()->extraList->front() : 0;
		invChanges->SendContainerChangedEvent(extraList, _containerRef, itemCopy.form(), itemCopy.count());
	}
	RE::ChestsLooted::SendEvent();
}


void LootMenu::TakeAllItems()
{
	if (!IsOpen() || !_containerRef || _displaySize <= 0) {
		return;
	}

	if (IsValidPickPocketTarget(_containerRef, RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
		return;
	}

	_canProcessInvChanges = true;

	UInt32 playSound = 5;
	for (auto& item : _invList) {
		TakeItem(item, item.count(), false, playSound);
		if (playSound) {
			--playSound;
		}
	}
	_invList.clear();
	SkipNextInput();
	_containerRef->ActivateRefChildren(RE::PlayerCharacter::GetSingleton());  // Trigger traps
	RE::ChestsLooted::SendEvent();

	_canProcessInvChanges = false;
}


InventoryList& LootMenu::GetInventoryList()
{
	return _invList;
}


void LootMenu::ParseInventory()
{
	_invList.parseInventory(_containerRef);
}


LootMenu::LootMenu(const char* a_swfPath) :
	_containerRef(0),
	_invList(),
	_actiText(""),
	_platform(Platform::kPC),
	_selectedIndex(0),
	_displaySize(0),
	_skipInputCount(0),
	_isContainerOpen(false),
	_isMenuOpen(false),
	_canProcessInvChanges(false),
	_isRegistered(false),
	_isEnabled(true)
{
	using ScaleModeType = RE::GFxMovieView::ScaleModeType;
	using Context = RE::InputMappingManager::Contexts;
	using Flag = RE::IMenu::Flag;

	RE::GFxLoader* loader = RE::GFxLoader::GetSingleton();
	if (loader->LoadMovie(this, view, a_swfPath, ScaleModeType::kShowAll, 0.0)) {
		flags = Flag::kDoNotDeleteOnClose | Flag::kDoNotPreventGameSave;
		context = Context::kInventory;
	}

	if (!view) {
		_FATALERROR("[FATAL ERROR] Lootmenu did not have a view, likely due to missing dependencies! Aborting process!\n");
		std::abort();
	}

	SetVisible(false);
	AddRef();	// Force persistence
}


LootMenu::~LootMenu()
{}


bool LootMenu::IsEnabled()
{
	return LootMenu::IsConstructed() && LootMenu::GetSingleton()->_isEnabled;
}


void LootMenu::ProcessMessageQueue()
{
	const char* msg = 0;
	while (!_messageQueue.empty()) {
		msg = _messageQueue.front();
		_messageQueue.pop();
		RE::DebugNotification(msg);
	}
}


bool LootMenu::SingleLootEnabled() const
{
	using BSKeyboardDevice = RE::BSKeyboardDevice;
	using BSWin32KeyboardDevice = RE::BSWin32KeyboardDevice;
	using BSGamepadDevice = RE::BSGamepadDevice;
	using BSWin32GamepadDevice = RE::BSWin32GamepadDevice;
	using DeviceType = RE::DeviceType;

	if (Settings::disableSingleLoot) {
		return false;
	}

	RE::InputManager* inputManager = RE::InputManager::GetSingleton();
	RE::BSWin32KeyboardDevice* keyboard = DYNAMIC_CAST(inputManager->keyboard, BSKeyboardDevice, BSWin32KeyboardDevice);
	if (keyboard && keyboard->IsEnabled()) {
		UInt32 singleLootKeyboard = GetSingleLootKey(DeviceType::kKeyboard);
		if (singleLootKeyboard != -1 && keyboard->IsPressed(singleLootKeyboard)) {
			return true;
		}
	}

	RE::BSGamepadDevice* gamepadHandle = 0;
	gamepadHandle = inputManager->GetGamepad();
	RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
	if (gamepad && gamepad->IsEnabled()) {
		UInt32 singleLootSprint = GetSingleLootKey(DeviceType::kGamepad);
		if (singleLootSprint != -1 && gamepad->IsPressed(singleLootSprint)) {
			return true;
		}
	}

	return false;
}


void LootMenu::PlayAnimation(const char* a_fromName, const char* a_toName) const
{
	using RE::NiControllerManager;

	if (Settings::disableAnimations) {
		return;
	}

	RE::NiNode* niNode = _containerRef->GetNiNode();
	if (!niNode) {
		return;
	}

	NiTimeController* controller = niNode->GetController();
	if (!controller) {
		return;
	}

	RE::NiControllerManager* manager = ni_cast(controller, NiControllerManager);
	if (!manager) {
		return;
	}

	RE::NiControllerSequence* fromSeq = manager->GetSequenceByName(a_fromName);
	RE::NiControllerSequence* toSeq = manager->GetSequenceByName(a_toName);
	if (!fromSeq || !toSeq) {
		return;
	}

	_containerRef->PlayAnimation(manager, toSeq, fromSeq);
}


void LootMenu::PlayAnimationOpen()
{
	if (_containerRef && !_isContainerOpen) {
		PlayAnimation("Close", "Open");
		if (_containerRef->formType != RE::FormType::ActorCharacter) {
			_containerRef->ActivateRefChildren(RE::PlayerCharacter::GetSingleton());  // Triggers traps
		}
		_isContainerOpen = true;
	}
}


void LootMenu::PlayAnimationClose()
{
	if (_containerRef && _isContainerOpen) {
		PlayAnimation("Open", "Close");
		_isContainerOpen = false;
	}
}


bool LootMenu::TakeItem(ItemData& a_item, UInt32 a_numItems, bool a_playAnim, bool a_playSound)
{
	using EventType = RE::PlayerCharacter::EventType;
	using RemoveType = RE::TESObjectREFR::RemoveType;
	using Archetype = RE::EffectSetting::Data::Archetype;

	bool manualUpdate = false;	// picking up dropped items doesn't disptach a container changed event

	// Locate item's extra list (if any)
	RE::BaseExtraList* xList = 0;
	if (a_item.entryData()->extraList && !a_item.entryData()->extraList->empty()) {
		xList = a_item.entryData()->extraList->front();
	}

	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

	// Pickup dropped items
	if (xList && xList->HasType(RE::ExtraDataType::kItemDropper)) {
		RE::TESObjectREFR* refItem = reinterpret_cast<RE::TESObjectREFR*>((uintptr_t)xList - offsetof(RE::TESObjectREFR, extraData));
		player->PickUpItem(refItem, 1, false, true);
		manualUpdate = true;
	} else {
		RemoveType lootMode = RemoveType::kTake;

		if (_containerRef->baseForm->Is(RE::FormType::NPC)) {
			// Dead body
			if (_containerRef->IsDead(false)) {
				player->PlayPickupEvent(a_item.form(), _containerRef->GetOwner(), _containerRef, EventType::kDeadBody);
				// Pickpocket
			} else {
				if (!TryToPickPocket(a_item, lootMode)) {
					return manualUpdate;
				}
			}
		} else {
			// Container
			player->PlayPickupEvent(a_item.form(), _containerRef->GetOwner(), _containerRef, EventType::kContainer);

			// Stealing
			if (_containerRef->IsOffLimits()) {
				lootMode = RemoveType::kSteal;
			}
		}

		// Remove projectile 3D
		RE::TESBoundObject* bound = static_cast<RE::TESBoundObject*>(a_item.form());
		if (bound) {
			bound->OnRemovedFrom(_containerRef);
		}

		if (_containerRef->baseForm->Is(RE::FormType::ActorCharacter)) {
			DispellWornItemEnchantments();
		} else {
			// Stealing
			if (_containerRef->IsOffLimits()) {
				player->SendStealAlarm(_containerRef, a_item.entryData()->type, a_numItems, a_item.value(), _containerRef->GetOwner(), true);
			}
		}

		if (a_playAnim) {
			PlayAnimationOpen();
		}
		if (a_playSound) {
			player->PlaySounds(a_item.form(), true, false);
		}
		if (!Settings::disableInvisDispell) {
			player->DispellEffectsWithArchetype(Archetype::kInvisibility, false);
		}
		UInt32 droppedHandle = 0;
		_containerRef->RemoveItem(droppedHandle, a_item.form(), a_numItems, lootMode, xList, player);
	}

	return manualUpdate;
}


bool LootMenu::TryToPickPocket(ItemData& a_item, RE::TESObjectREFR::RemoveType& a_lootMode) const
{
	using EventType = RE::PlayerCharacter::EventType;
	using RemoveType = RE::TESObjectREFR::RemoveType;

	RE::Actor* target = static_cast<RE::Actor*>(_containerRef);
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	bool pickSuccess = player->TryToPickPocket(target, a_item.entryData(), a_item.count(), true);
	player->PlayPickupEvent(a_item.entryData()->type, _containerRef->GetActorOwner(), _containerRef, EventType::kThief);
	a_lootMode = RemoveType::kSteal;
	if (!pickSuccess) {
		return false;
	} else {
		RE::ItemsPickpocketed::SendEvent(a_item.count());
		return true;
	}
}


void LootMenu::DispellWornItemEnchantments() const
{
	RE::Actor* actor = static_cast<RE::Actor*>(_containerRef);
	if (actor->processManager) {
		actor->DispelWornItemEnchantments();
		actor->processManager->UpdateEquipment_Hooked(actor);
	}
}


UInt32 LootMenu::GetSingleLootKey(RE::DeviceType a_deviceType) const
{
	RE::BSFixedString str = _singleLootMapping.c_str();
	return RE::InputMappingManager::GetSingleton()->GetMappedKey(str, a_deviceType);
}


LootMenu*				LootMenu::_singleton = 0;
std::string				LootMenu::_singleLootMapping = "";
std::string				LootMenu::_takeMapping = "";
std::string				LootMenu::_takeAllMapping = "";
std::string				LootMenu::_searchMapping = "";
std::queue<const char*>	LootMenu::_messageQueue;
