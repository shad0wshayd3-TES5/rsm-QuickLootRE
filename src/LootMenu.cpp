#include "LootMenu.h"

#include "skse64_common/SafeWrite.h"

#include <array>
#include <cstdlib>  // abort
#include <queue>
#include <string>

#include "ActivatePerkEntryVisitor.h"
#include "Forms.h"
#include "Hooks.h"
#include "ItemData.h"
#include "InventoryList.h"
#include "Registration.h"
#include "Settings.h"
#include "Utility.h"

#include "SKSE/API.h"
#include "RE/Skyrim.h"


namespace
{
	StyleMap::StyleMap()
	{
		std::string key;

		key = "dialogue";
		insert({ key, Style::kDialogue });

		key = "default";
		insert({ key, Style::kDefault });
	}
}


RE::IMenu::Result LootMenu::ProcessMessage(RE::UIMessage* a_message)
{
	using UIMessage = RE::UIMessage::Message;

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

	ControlMethod inputMethod;
	switch (a_event->deviceType) {
	case DeviceType::kGamepad:
		inputMethod = ControlMethod::kController;
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
		inputMethod = ControlMethod::kPC;
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
		inputMethod = ControlMethod::kPC;
		if (controlID == strHolder->zoomIn) {
			ModSelectedIndex(-1);
		} else if (controlID == strHolder->zoomOut) {
			ModSelectedIndex(1);
		}
		break;
	}

	if (_controlMethod != inputMethod) {
		_controlMethod = inputMethod;
		UpdateButtonIcons(_controlMethod == ControlMethod::kController);
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
		_ERROR("Invalid message (%i)", a_msg);
		break;
	}

	if (loot->IsOpen()) {
		ProcessMessageQueue();
	}
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
		Dispatch<SetSelectedIndexDelegate>(_selectedIndex);
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

	if (!_isEnabled) {
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
	auto droppedList = containerRef->extraData.GetByType<RE::ExtraDroppedItemList>();
	if (Settings::disableIfEmpty && numItems <= 0 && (!droppedList || droppedList->handles.empty())) {
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


void LootMenu::TakeItemStack()
{
	if (!IsOpen() || !_containerRef || _displaySize <= 0) {
		return;
	}

	ItemData itemCopy(_invList[_selectedIndex]);

	auto numItems = itemCopy.GetCount();
	if (numItems > 1 && IsSingleLootEnabled()) {
		numItems = 1;
	}

	if (TakeItem(itemCopy, numItems, true, true)) {
		auto player = RE::PlayerCharacter::GetSingleton();
		auto invChanges = player->GetInventoryChanges();
		auto extraList = itemCopy.GetEntryData()->extraList ? itemCopy.GetEntryData()->extraList->front() : 0;
		invChanges->SendContainerChangedEvent(extraList, _containerRef, itemCopy.GetForm(), itemCopy.GetCount());
	}
	RE::ChestsLooted::SendEvent();
}


void LootMenu::TakeAllItems()
{
	using DefaultObjects = RE::BGSDefaultObjectManager::DefaultObjects;

	if (!IsOpen() || !_containerRef || _displaySize <= 0) {
		return;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (IsValidPickPocketTarget(_containerRef, player->IsSneaking())) {
		return;
	}

	_canProcessInvChanges = true;

	std::array<RE::BGSSoundDescriptorForm*, 5> descriptors = { 0 };
	auto dObjManager = RE::BGSDefaultObjectManager::GetSingleton();
	descriptors[0] = dObjManager->GetObject<RE::BGSSoundDescriptorForm>(DefaultObjects::kITMGenericUpSD);
	descriptors[1] = dObjManager->GetObject<RE::BGSSoundDescriptorForm>(DefaultObjects::kITMGenericWeaponUpSD);
	descriptors[2] = dObjManager->GetObject<RE::BGSSoundDescriptorForm>(DefaultObjects::kITMGenericArmorUpSD);
	descriptors[3] = dObjManager->GetObject<RE::BGSSoundDescriptorForm>(DefaultObjects::kITMGenericBookUpSD);
	descriptors[4] = dObjManager->GetObject<RE::BGSSoundDescriptorForm>(DefaultObjects::kITMGenericIngredientUpSD);

	auto soundManager = RE::BSAudioManager::GetSingleton();
	for (auto& descriptor : descriptors) {
		if (descriptor) {
			soundManager->Play(descriptor);
		}
	}

	for (auto& item : _invList) {
		TakeItem(item, item.GetCount(), false, false);
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
	_controlMethod(ControlMethod::kPC),
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
		_FATALERROR("Lootmenu did not have a view due to missing dependencies! Aborting process!\n");
		MessageBoxA(NULL, "Lootmenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
		std::abort();
	}

	SetVisible(false);
	AddRef();	// Force persistence
	Dispatch<SetupDelegate>();
	Dispatch<SwitchStyleDelegate>(GetStyle());
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


void LootMenu::OnMenuOpen()
{
	if (!_containerRef) {
		return;
	}

	auto inputManager = RE::InputManager::GetSingleton();
	if (inputManager->IsGamepadEnabled()) {
		_controlMethod = ControlMethod::kController;
		UpdateButtonIcons(true);
	} else {
		_controlMethod = ControlMethod::kPC;
		UpdateButtonIcons(false);
	}

	_selectedIndex = 0;
	_skipInputCount = 0;
	_isMenuOpen = true;
	Dispatch<SetContainerDelegate>(_selectedIndex);

	auto player = RE::PlayerCharacter::GetSingleton();
	if (IsValidPickPocketTarget(_containerRef, player->IsSneaking())) {
		Dispatch<SetVisibleButtonsDelegate>(true, false, true);
	} else {
		Dispatch<SetVisibleButtonsDelegate>(true, true, true);
	}

	Dispatch<OpenContainerDelegate>();
	SetVisible(true);
	ProcessMessageQueue();
}


void LootMenu::OnMenuClose()
{
	if (IsOpen()) {
		SetVisible(false);
		_isMenuOpen = false;
		Dispatch<CloseContainerDelegate>();
		PlayAnimationClose();
	}
}


Style LootMenu::GetStyle() const
{
	static StyleMap styleMap;
	auto it = styleMap.find(Settings::interfaceStyle);
	if (it != styleMap.end()) {
		return it->second;
	} else {
		_ERROR("Invalid style (%s)!", Settings::interfaceStyle.c_str());
		_ERROR("Using default!\n");
		return Style::kDefault;
	}
}


void LootMenu::UpdateButtonIcons(bool a_controller) const
{
	UInt32 take;
	UInt32 takeAll;
	UInt32 search;
	if (a_controller) {
		GetGamepadButtonID(take, _takeMapping);
		GetGamepadButtonID(takeAll, _takeAllMapping);
		GetGamepadButtonID(search, _searchMapping);
	} else {
		GetPCButtonID(take, _takeMapping);
		GetPCButtonID(takeAll, _takeAllMapping);
		GetPCButtonID(search, _searchMapping);
	}
	Dispatch<UpdateButtonIconsDelegate>(take, takeAll, search);
}


void LootMenu::GetGamepadButtonID(UInt32& a_key, const std::string_view& a_mapping) const
{
	using Key = RE::BSWin32GamepadDevice::Key;

	auto input = RE::InputMappingManager::GetSingleton();
	a_key = input->GetMappedKey(a_mapping, RE::DeviceType::kGamepad);
	switch (a_key) {
	case Key::kUp:
		a_key = 0;
		break;
	case Key::kDown:
		a_key = 1;
		break;
	case Key::kLeft:
		break;
		a_key = 2;
	case Key::kRight:
		a_key = 3;
		break;
	case Key::kStart:
		a_key = 4;
		break;
	case Key::kBack:
		a_key = 5;
		break;
	case Key::kLeftThumb:
		a_key = 6;
		break;
	case Key::kRightThumb:
		a_key = 7;
		break;
	case Key::kLeftShoulder:
		a_key = 8;
		break;
	case Key::kRightShoulder:
		a_key = 9;
		break;
	case Key::kA:
		a_key = 10;
		break;
	case Key::kB:
		a_key = 11;
		break;
	case Key::kX:
		a_key = 12;
		break;
	case Key::kY:
		a_key = 13;
		break;
	case Key::kLeftTrigger:
		a_key = 14;
		break;
	case Key::kRightTrigger:
		a_key = 15;
		break;
	default:
		a_key = kInvalidButton;
		break;
	}

	if (a_key == kInvalidButton) {
		a_key = kESC;
	} else {
		a_key += kGamepadOffset;
	}
}


void LootMenu::GetPCButtonID(UInt32& a_key, const std::string_view& a_mapping) const
{
	auto input = RE::InputMappingManager::GetSingleton();
	a_key = input->GetMappedKey(a_mapping, RE::DeviceType::kKeyboard);
	if (a_key == kInvalidButton) {
		a_key = input->GetMappedKey(a_mapping, RE::DeviceType::kMouse);
		if (a_key == kInvalidButton) {
			a_key = kESC;
		} else {
			a_key += kMouseOffset;
		}
	} else {
		a_key += kKeyboardOffset;
	}
}


bool LootMenu::IsSingleLootEnabled() const
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
		if (singleLootSprint != kInvalidButton && gamepad->IsPressed(singleLootSprint)) {
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
	if (a_item.GetEntryData()->extraList && !a_item.GetEntryData()->extraList->empty()) {
		xList = a_item.GetEntryData()->extraList->front();
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
				player->PlayPickupEvent(a_item.GetForm(), _containerRef->GetOwner(), _containerRef, EventType::kDeadBody);
				// Pickpocket
			} else {
				if (!TryToPickPocket(a_item, lootMode)) {
					return manualUpdate;
				}
			}
		} else {
			// Container
			player->PlayPickupEvent(a_item.GetForm(), _containerRef->GetOwner(), _containerRef, EventType::kContainer);

			// Stealing
			if (_containerRef->IsOffLimits()) {
				lootMode = RemoveType::kSteal;
			}
		}

		// Remove projectile 3D
		auto bound = static_cast<RE::TESBoundObject*>(a_item.GetForm());
		if (bound) {
			bound->OnRemovedFrom(_containerRef);
		}

		if (_containerRef->baseForm->Is(RE::FormType::ActorCharacter)) {
			DispellWornItemEnchantments();
		} else {
			// Stealing
			if (_containerRef->IsOffLimits()) {
				player->SendStealAlarm(_containerRef, a_item.GetEntryData()->type, a_numItems, a_item.GetValue(), _containerRef->GetOwner(), true);
			}
		}

		if (a_playAnim) {
			PlayAnimationOpen();
		}
		if (a_playSound) {
			player->PlaySounds(a_item.GetForm(), true, false);
		}
		if (!Settings::disableInvisDispell) {
			player->DispellEffectsWithArchetype(Archetype::kInvisibility, false);
		}
		RE::RefHandle droppedHandle = 0;
		_containerRef->RemoveItem(droppedHandle, a_item.GetForm(), a_numItems, lootMode, xList, player);
	}

	return manualUpdate;
}


bool LootMenu::TryToPickPocket(ItemData& a_item, RE::TESObjectREFR::RemoveType& a_lootMode) const
{
	using EventType = RE::PlayerCharacter::EventType;
	using RemoveType = RE::TESObjectREFR::RemoveType;

	auto target = static_cast<RE::Actor*>(_containerRef);
	auto player = RE::PlayerCharacter::GetSingleton();
	auto pickSuccess = player->TryToPickPocket(target, a_item.GetEntryData(), a_item.GetCount(), true);
	player->PlayPickupEvent(a_item.GetEntryData()->type, _containerRef->GetActorOwner(), _containerRef, EventType::kThief);
	a_lootMode = RemoveType::kSteal;
	if (!pickSuccess) {
		return false;
	} else {
		RE::ItemsPickpocketed::SendEvent(a_item.GetCount());
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
	auto mm = RE::InputMappingManager::GetSingleton();
	return mm->GetMappedKey(_singleLootMapping, a_deviceType);
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
