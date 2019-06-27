#include "LootMenu.h"

#include "skse64_common/SafeWrite.h"  // SafeWrite64
#include "skse64/GameMenus.h"  // UIMessage
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
#include "Registration.h"  // OnContainerOpenAnim, OnContainerCloseAnim
#include "Settings.h"  // Settings
#include "Utility.h"  // IsValidPickPocketTarget

#include "SKSE/API.h"
#include "RE/Skyrim.h"


RE::IMenu::Result LootMenu::ProcessMessage(RE::UIMessage* a_message)
{
	using UIMessage = RE::UIMessage::Message;

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
	using Gamepad = RE::BSWin32GamepadDevice::Key;
	using Mouse = RE::BSWin32MouseDevice::Key;

	if (IsOpen() && a_event->eventType == EventType::kButton) {
		auto button = static_cast<RE::ButtonEvent*>(a_event);

		auto& controlID = a_event->GetControlID();
		auto strHolder = RE::InputStringHolder::GetSingleton();
		if (controlID == strHolder->sneak) {
			return true;
		}

		switch (a_event->deviceType) {
		case DeviceType::kGamepad:
			{
				auto keyMask = static_cast<Gamepad>(button->keyMask);
				return (keyMask == Gamepad::kUp || keyMask == Gamepad::kDown);
			}
			break;
		case DeviceType::kMouse:
			{
				auto keyMask = static_cast<Mouse>(button->keyMask);
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
	using Gamepad = RE::BSWin32GamepadDevice::Key;
	using Mouse = RE::BSWin32MouseDevice::Key;

	if (!a_event->IsDown()) {
		return true;
	}

	auto& controlID = a_event->GetControlID();
	auto strHolder = RE::InputStringHolder::GetSingleton();
	if (controlID == strHolder->sneak) {
		Close();
		SkipNextInput();
		auto player = RE::PlayerCharacter::GetSingleton();
		if (CanOpen(_containerRef, !player->IsSneaking())) {
			Open();
		}
		return true;
	}

	switch (a_event->deviceType) {
	case DeviceType::kGamepad:
		_platform = Platform::kOther;
		Register(Scaleform::kSetPlatform);
		Register(Scaleform::kUpdateButtons);
		switch (static_cast<Gamepad>(a_event->keyMask)) {
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
	if (_singleton) {
		_singleton->Release();
		_singleton = 0;
	}
}


const RE::BSFixedString& LootMenu::GetName()
{
	static RE::BSFixedString name = "LootMenu";
	return name;
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
	auto loot = LootMenu::GetSingleton();

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
			auto state = loot->IsEnabled() ? enabled : disabled;
			_messageQueue.push(state);
			ProcessMessageQueue();
		}
		break;
	default:
		_ERROR("[ERROR] Invalid message (%i)", a_msg);
		break;
	}

	if (loot->IsOpen()) {
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
		auto uiManager = RE::UIManager::GetSingleton();
		uiManager->AddMessage(GetName(), RE::UIMessage::Message::kOpen, 0);
	}
}


void LootMenu::Close() const
{
	auto uiManager = RE::UIManager::GetSingleton();
	uiManager->AddMessage(GetName(), RE::UIMessage::Message::kClose, 0);
}


void LootMenu::SetVisible(bool a_visible)
{
	auto mc = RE::MenuControls::GetSingleton();
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

	if (!GetEnabled()) {
		return 0;
	}

	if (!a_ref || !a_ref->baseForm) {
		return 0;
	}

	auto mm = RE::MenuManager::GetSingleton();
	auto uiStrHolder = RE::UIStringHolder::GetSingleton();
	if (mm->GameIsPaused() || mm->CrosshairIsPaused() || mm->GetMenu(uiStrHolder->dialogueMenu)) {
		return 0;
	}

	auto mappingManager = RE::InputMappingManager::GetSingleton();
	if (!mappingManager->IsMovementControlsEnabled()) {
		return 0;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
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
			RE::RefHandle refHandle = 0;
			if (a_ref->extraData.GetAshPileRefHandle(refHandle) && refHandle != *g_invalidRefHandle) {
				RE::TESObjectREFRPtr refPtr;
				if (RE::TESObjectREFR::LookupByHandle(refHandle, refPtr)) {
					containerRef = refPtr.get();
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
		auto target = static_cast<RE::Actor*>(a_ref);
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

	auto numItems = containerRef->GetNumItems();

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
	auto task = SKSE::GetTaskInterface();
	switch (a_reg) {
	case Scaleform::kSetKeyMappings:
		task->AddUITask(new SetKeyMappingsDelegate());
		break;
	case Scaleform::kSetPlatform:
		task->AddUITask(new SetPlatformDelegate());
		break;
	case Scaleform::kSetSelectedIndex:
		task->AddUITask(new SetSelectedIndexDelegate());
		break;
	case Scaleform::kSetup:
		task->AddUITask(new SetupDelegate());
		break;
	case Scaleform::kSetContainer:
		task->AddUITask(new SetContainerDelegate());
		break;
	case Scaleform::kOpenContainer:
		task->AddUITask(new OpenContainerDelegate());
		break;
	case Scaleform::kCloseContainer:
		task->AddUITask(new CloseContainerDelegate());
		break;
	case Scaleform::kUpdateButtons:
		task->AddUITask(new UpdateButtonsDelegate());
		break;
	case Scaleform::kHideButtons:
		task->AddUITask(new HideButtonsDelegate());
		break;
	case Scaleform::kSwitchStyle:
		task->AddUITask(new SwitchStyleDelegate());
		break;
	default:
		_ERROR("[ERROR] Invalid registration (%i)!\n", a_reg);
		break;
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
	if (!_containerRef) {
		return;
	}

	auto gamepadHandle = RE::InputManager::GetSingleton()->GetGamepad();
	auto gamepad = skyrim_cast<RE::BSWin32GamepadDevice*>(gamepadHandle);
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

	auto player = RE::PlayerCharacter::GetSingleton();
	if (IsValidPickPocketTarget(_containerRef, player->IsSneaking())) {
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

	auto numItems = itemCopy.count();
	if (numItems > 1 && SingleLootEnabled()) {
		numItems = 1;
	}

	if (TakeItem(itemCopy, numItems, true, true)) {
		auto player = RE::PlayerCharacter::GetSingleton();
		auto invChanges = player->GetInventoryChanges();
		auto extraList = itemCopy.entryData()->extraList ? itemCopy.entryData()->extraList->front() : 0;
		invChanges->SendContainerChangedEvent(extraList, _containerRef, itemCopy.form(), itemCopy.count());
	}
	RE::ChestsLooted::SendEvent();
}


void LootMenu::TakeAllItems()
{
	if (!IsOpen() || !_containerRef || _displaySize <= 0) {
		return;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (IsValidPickPocketTarget(_containerRef, player->IsSneaking())) {
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
	_containerRef->ActivateRefChildren(player);  // Trigger traps
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
	using Context = RE::InputMappingManager::Context;
	using Flag = RE::IMenu::Flag;

	auto loader = RE::GFxLoader::GetSingleton();
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
	using DeviceType = RE::DeviceType;

	if (Settings::disableSingleLoot) {
		return false;
	}

	auto inputManager = RE::InputManager::GetSingleton();
	auto keyboard = skyrim_cast<RE::BSWin32KeyboardDevice*>(inputManager->keyboard);
	if (keyboard && keyboard->IsEnabled()) {
		UInt32 singleLootKeyboard = GetSingleLootKey(DeviceType::kKeyboard);
		if (singleLootKeyboard != -1 && keyboard->IsPressed(singleLootKeyboard)) {
			return true;
		}
	}

	RE::BSGamepadDevice* gamepadHandle = 0;
	gamepadHandle = inputManager->GetGamepad();
	auto gamepad = skyrim_cast<RE::BSWin32GamepadDevice*>(gamepadHandle);
	if (gamepad && gamepad->IsEnabled()) {
		auto singleLootSprint = GetSingleLootKey(DeviceType::kGamepad);
		if (singleLootSprint != RE::InputMappingManager::kInvalid && gamepad->IsPressed(singleLootSprint)) {
			return true;
		}
	}

	return false;
}


void LootMenu::PlayAnimation(const char* a_fromName, const char* a_toName) const
{
	if (Settings::disableAnimations) {
		return;
	}

	auto niNode = _containerRef->GetNiNode();
	if (!niNode) {
		return;
	}

	auto controller = niNode->GetController();
	if (!controller) {
		return;
	}

	auto manager = skyrim_cast<RE::NiControllerManager*>(controller);
	if (!manager) {
		return;
	}

	auto fromSeq = manager->GetSequenceByName(a_fromName);
	auto toSeq = manager->GetSequenceByName(a_toName);
	if (!fromSeq || !toSeq) {
		return;
	}

	_containerRef->PlayAnimation(manager, toSeq, fromSeq);
}


void LootMenu::PlayAnimationOpen()
{
	if (_containerRef && !_isContainerOpen) {
		PlayAnimation("Close", "Open");

		if (!Settings::disableAnimations) {
			OnContainerOpenAnim::GetSingleton()->QueueEvent();
		}

		if (_containerRef->formType != RE::FormType::ActorCharacter) {
			auto player = RE::PlayerCharacter::GetSingleton();
			_containerRef->ActivateRefChildren(player);  // Triggers traps
		}

		_isContainerOpen = true;
	}
}


void LootMenu::PlayAnimationClose()
{
	if (_containerRef && _isContainerOpen) {
		PlayAnimation("Open", "Close");

		if (!Settings::disableAnimations) {
			auto dispatcher = OnContainerCloseAnim::GetSingleton();
			dispatcher->QueueEvent();
		}

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

	auto player = RE::PlayerCharacter::GetSingleton();

	// Pickup dropped items
	if (xList && xList->HasType(RE::ExtraDataType::kItemDropper)) {
		auto refItem = reinterpret_cast<RE::TESObjectREFR*>((std::uintptr_t)xList - offsetof(RE::TESObjectREFR, extraData));
		player->PickUpItem(refItem, 1, false, true);
		manualUpdate = true;
	} else {
		auto lootMode = RemoveType::kTake;

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
		auto bound = static_cast<RE::TESBoundObject*>(a_item.form());
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
		RE::RefHandle droppedHandle = 0;
		_containerRef->RemoveItem(droppedHandle, a_item.form(), a_numItems, lootMode, xList, player);
	}

	return manualUpdate;
}


bool LootMenu::TryToPickPocket(ItemData& a_item, RE::TESObjectREFR::RemoveType& a_lootMode) const
{
	using EventType = RE::PlayerCharacter::EventType;
	using RemoveType = RE::TESObjectREFR::RemoveType;

	auto target = static_cast<RE::Actor*>(_containerRef);
	auto player = RE::PlayerCharacter::GetSingleton();
	auto pickSuccess = player->TryToPickPocket(target, a_item.entryData(), a_item.count(), true);
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
	auto actor = static_cast<RE::Actor*>(_containerRef);
	if (actor->processManager) {
		actor->DispelWornItemEnchantments();
		actor->processManager->UpdateEquipment_Hooked(actor);
	}
}


UInt32 LootMenu::GetSingleLootKey(RE::DeviceType a_deviceType) const
{
	RE::BSFixedString str = _singleLootMapping.c_str();
	auto mm = RE::InputMappingManager::GetSingleton();
	return mm->GetMappedKey(str, a_deviceType);
}


bool LootMenu::IsEnabled() const
{
	return _isEnabled;
}


decltype(LootMenu::_singleton)			LootMenu::_singleton = 0;
decltype(LootMenu::_singleLootMapping)	LootMenu::_singleLootMapping = "";
decltype(LootMenu::_takeMapping)		LootMenu::_takeMapping = "";
decltype(LootMenu::_takeAllMapping)		LootMenu::_takeAllMapping = "";
decltype(LootMenu::_searchMapping)		LootMenu::_searchMapping = "";
decltype(LootMenu::_messageQueue)		LootMenu::_messageQueue;
