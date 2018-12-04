#include "LootMenu.h"

#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameSettings.h"  // g_gameSettingCollection
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/NiRTTI.h"  // ni_cast
#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include <queue>  // queue
#include <string>  // string

#include "ActivatePerkEntryVisitor.h"  // ActivatePerkEntryVisitor
#include "Delegates.h"
#include "Forms.h"  // FACTFormID
#include "Hooks.h"  // SendItemsPickPocketedEvent()
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList
#include "Settings.h"  // Settings
#include "Utility.h"  // IsValidPickPocketTarget()

#include "RE/Actor.h"  // Actor
#include "RE/ActorProcessManager.h"  // ActorProcessManager
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/BSWin32MouseDevice.h"  // BSWin32MouseDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/GFxMovieDef.h"  // GFxMovieDef
#include "RE/GFxMovieView.h"  // GFxMovieView
#include "RE/GFxLoader.h"  // GFxLoader
#include "RE/IMenu.h"  // IMenu
#include "RE/InputEvent.h"  // InputEvent
#include "RE/InputManager.h"  // InputManager
#include "RE/InputMappingManager.h"  // InputMappingManager
#include "RE/InputStringHolder.h"  // InputStringHolder
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/MenuControls.h"  // MenuControls
#include "RE/MenuManager.h"  // MenuManager
#include "RE/NiControllerManager.h"  // NiControllerManager
#include "RE/NiNode.h"  // NiNode
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESBoundObject.h"  // TESBoundObject
#include "RE/TESFaction.h"  // TESFaction
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESRace.h"  // TESRace
#include "RE/UIManager.h"  // UIManager
#include "RE/UIStringHolder.h"  // UIStringHolder

class TESObjectREFR;


namespace QuickLootRE
{
	RE::IMenu* LootMenuCreator::Create()
	{
		void* p = ScaleformHeap_Allocate(sizeof(LootMenu));
		if (p) {
			LootMenu::_singleton = new (p) LootMenu(LootMenu::GetName().c_str());
			return LootMenu::_singleton;
		} else {
			return 0;
		}
	}


	LootMenu::LootMenu(const char* a_swfPath)
	{
		typedef RE::GFxMovieView::ScaleModeType ScaleModeType;
		typedef RE::InputMappingManager::Context	Context;

		RE::GFxLoader* loader = RE::GFxLoader::GetSingleton();
		if (loader->LoadMovie(this, view, a_swfPath, ScaleModeType::kScaleModeType_ShowAll, 0.0)) {
			flags = Flag(kFlag_DoNotDeleteOnClose | kFlag_DoNotPreventGameSave | kFlag_Unk10000);
			context = Context::kContext_Inventory;
		}
	}


	LootMenu::~LootMenu()
	{
		_singleton = 0;
		_selectedIndex = 0;
		_displaySize = 0;
		_skipInputCount = 0;
		_containerRef = 0;
		_isContainerOpen = false;
		_isMenuOpen = false;
		_inTakeAllMode = false;
		_isRegistered = false;
		_platform = kPlatform_PC;
		_actiText = "";
	}


	LootMenu* LootMenu::GetSingleton()
	{
		return _singleton;
	}


	SInt32 LootMenu::GetSelectedIndex()
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
			Register(kScaleform_SetSelectedIndex);
		}
	}


	void LootMenu::SetDisplaySize(SInt32 a_size)
	{
		_displaySize = a_size;
	}


	bool LootMenu::ShouldSkipNextInput()
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


	RE::TESObjectREFR* LootMenu::GetContainerRef()
	{
		return _containerRef;
	}


	void LootMenu::ClearContainerRef()
	{
		if (_singleton) {
			_singleton->PlayAnimationClose();
		}
		_containerRef = 0;
	}


	bool LootMenu::IsConstructed()
	{
		return _singleton && _singleton->view;
	}


	bool LootMenu::IsOpen()
	{
		return _isMenuOpen;
	}


	bool LootMenu::IsVisible()
	{
		return IsConstructed() && _singleton->view->GetVisible();;
	}


	bool LootMenu::InTakeAllMode()
	{
		return _inTakeAllMode;
	}


	bool LootMenu::GetEnabled()
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


	LootMenu::Platform LootMenu::GetPlatform()
	{
		return _platform;
	}


	RE::BSFixedString LootMenu::GetName()
	{
		static RE::BSFixedString name = "LootMenu";
		return name;
	}


	const char* LootMenu::GetActiText()
	{
		return _actiText.c_str();
	}


	void LootMenu::SetActiText(const char* a_actiText)
	{
		_actiText = a_actiText;
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


	void LootMenu::Open()
	{
		if (_isEnabled) {
			RE::UIManager::GetSingleton()->AddMessage(GetName(), UIMessage::kMessage_Open, 0);
		}
	}


	void LootMenu::Close()
	{
		RE::UIManager::GetSingleton()->AddMessage(GetName(), UIMessage::kMessage_Close, 0);
	}


	void LootMenu::SetVisible(bool a_visible)
	{
		typedef RE::InputMappingManager::Context Context;

		RE::MenuControls* mc = RE::MenuControls::GetSingleton();
		if (_singleton && _singleton->view) {
			_singleton->view->SetVisible(a_visible);
			if (a_visible && !_isRegistered) {
				mc->RegisterHandler(_singleton);
				_isRegistered = true;
			} else if (!a_visible && _isRegistered) {
				mc->RemoveHandler(_singleton);
				_isRegistered = false;
			}
		}
	}


	void LootMenu::SetContainerRef(RE::TESObjectREFR* a_ref)
	{
		_containerRef = a_ref;
	}


	bool LootMenu::CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking)
	{
		typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

		static RE::BSFixedString strAnimationDriven = "bAnimationDriven";

		if (!LootMenu::GetEnabled()) {
			return false;
		}

		if (!a_ref || !a_ref->baseForm) {
			return false;
		}

		RE::MenuManager* mm = RE::MenuManager::GetSingleton();
		RE::UIStringHolder* strHolder = RE::UIStringHolder::GetSingleton();
		if (mm->GameIsPaused() || mm->CrosshairIsPaused() || mm->GetMenu(strHolder->dialogueMenu)) {
			return false;
		}

		RE::InputMappingManager* mappingManager = RE::InputMappingManager::GetSingleton();
		if (!mappingManager->IsMovementControlsEnabled()) {
			return false;
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		if (player->GetGrabbedRef() || player->GetActorInFavorState() || player->IsInKillMove()) {
			return false;
		}

		bool bAnimationDriven;
		if (player->GetAnimationVariableBool(strAnimationDriven, bAnimationDriven) && bAnimationDriven) {
			return false;
		}

		if (Settings::disableInCombat && player->IsInCombat()) {
			return false;
		}

		if (Settings::disableTheft && a_ref->IsOffLimits()) {
			return false;
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
			break;
		}
		case RE::FormType::Container:
			if (!a_ref->IsLocked()) {
				containerRef = a_ref;
			}
			break;
		case RE::FormType::NPC:
			RE::Actor* target = static_cast<RE::Actor*>(a_ref);
			if (Settings::disableForAnimals && target->GetRace()->HasKeyword(ActorTypeAnimal)) {
				return false;
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
			return false;
		}

		UInt32 numItems = containerRef->GetNumItems(false, false);

		if (Settings::disableIfEmpty && numItems <= 0) {
			return false;
		}

		if (Settings::disableForActiOverride && player->CanProcessEntryPointPerkEntry(EntryPointType::kEntryPoint_Activate)) {
			ActivatePerkEntryVisitor visitor(player, containerRef);
			player->VisitEntryPointPerkEntries(EntryPointType::kEntryPoint_Activate, visitor);
			if (visitor.GetResult()) {
				return false;
			}
		}

		_containerRef = containerRef;

		return true;
	}


	void LootMenu::Register(Scaleform a_reg)
	{
		if (LootMenu::IsConstructed()) {
			switch (a_reg) {
			case kScaleform_SetKeyMappings:
				AllocateAndDispatch<SetKeyMappingsUIDelegate>();
				break;
			case kScaleform_SetPlatform:
				AllocateAndDispatch<SetPlatformUIDelegate>();
				break;
			case kScaleform_SetSelectedIndex:
				AllocateAndDispatch<SetSelectedIndexUIDelegate>();
				break;
			case kScaleform_Setup:
				AllocateAndDispatch<SetupUIDelegate>();
				break;
			case kScaleform_SetContainer:
				AllocateAndDispatch<SetContainerUIDelegate>();
				break;
			case kScaleform_OpenContainer:
				AllocateAndDispatch<OpenContainerUIDelegate>();
				break;
			case kScaleform_CloseContainer:
				AllocateAndDispatch<CloseContainerUIDelegate>();
				break;
			case kScaleform_UpdateButtons:
				AllocateAndDispatch<UpdateButtonsUIDelegate>();
				break;
			case kScaleform_HideButtons:
				AllocateAndDispatch<HideButtonsUIDelegate>();
				break;
			case kScaleform_SwitchStyle:
				AllocateAndDispatch<SwitchStyleTaskDelegate>();
				break;
			default:
				_ERROR("[ERROR] Invalid registration (%i)!\n", a_reg);
			}
		} else {
			_ERROR("[ERROR] The LootMenu has not been constructed!\n");
		}
	}


	void LootMenu::QueueMessage(Message a_msg)
	{
		switch (a_msg) {
		case kMessage_NoInputLoaded:
			_messageQueue.push("[LootMenu] ERROR: Input mapping conflicts detected! No inputs mapped!");
			break;
		case kMessage_HookShareMissing:
			_messageQueue.push("[LootMenu] ERROR: Hook Share SSE is not loaded!");
			break;
		case kMessage_HookShareIncompatible:
			_messageQueue.push("[LootMenu] ERROR: Hook Share SSE is an incompatible version!");
			break;
		case kMessage_MissingDependencies:
			_messageQueue.push("[LootMenu] ERROR: LootMenu is missing a view! Dependencies were not loaded!");
			ProcessMessageQueue();
			break;
		case kMessage_LootMenuToggled:
		{
			static const char* enabled = "[LootMenu] LootMenu enabled";
			static const char* disabled = "[LootMenu] LootMenu disabled";
			const char* state = _isEnabled ? enabled : disabled;
			_messageQueue.push(state);
			ProcessMessageQueue();
			break;
		}
		default:
			_ERROR("[ERROR] Invalid message (%i)", a_msg);
		}

		if (IsOpen()) {
			ProcessMessageQueue();
		}
	}


	LootMenu::Style LootMenu::GetStyle()
	{
		if (Settings::interfaceStyle == "dialogue") {
			return kStyle_Dialogue;
		} else {
			if (Settings::interfaceStyle != "default") {
				_ERROR("Invalid style (%s)!", Settings::interfaceStyle.c_str());
				_ERROR("Using default!\n");
			}
			return kStyle_Default;
		}
	}


	RE::IMenu::Result LootMenu::ProcessMessage(UIMessage* a_message)
	{
		if (!view) {
			_FATALERROR("[FATAL ERROR] LootMenu is missing a view! Dependencies were not loaded!\n");
			QueueMessage(kMessage_MissingDependencies);
			return Result::kResult_NotProcessed;
		}

		if (!Settings::isApplied) {
			Register(kScaleform_Setup);
			Register(kScaleform_SwitchStyle);
		}

		switch (a_message->message) {
		case UIMessage::kMessage_Open:
			OnMenuOpen();
			break;
		case UIMessage::kMessage_Close:
			OnMenuClose();
			break;
		}

		return Result::kResult_NotProcessed;
	}


	void LootMenu::Render()
	{
		if (IsOpen()) {
			view->Display();
		}
	}


	bool LootMenu::CanProcess(RE::InputEvent* a_event)
	{
		typedef RE::InputEvent::DeviceType			DeviceType;
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

		if (IsOpen() && a_event->eventType == InputEvent::kEventType_Button) {
			RE::ButtonEvent* button = static_cast<RE::ButtonEvent*>(a_event);

			RE::BSFixedString controlID = a_event->GetControlID();
			RE::InputStringHolder* strHolder = RE::InputStringHolder::GetSingleton();
			if (controlID == strHolder->sneak) {
				return true;
			}

			switch (a_event->deviceType) {
			case DeviceType::kDeviceType_Gamepad:
				return (button->keyMask == Gamepad::kGamepad_Up || button->keyMask == Gamepad::kGamepad_Down);
			case DeviceType::kDeviceType_Mouse:
				return (button->keyMask == Mouse::kMouse_WheelDown || button->keyMask == Mouse::kMouse_WheelUp);
			case DeviceType::kDeviceType_Keyboard:
				return (controlID == strHolder->zoomIn || controlID == strHolder->zoomOut);
			}
		}
		return false;
	}


	bool LootMenu::ProcessButton(RE::ButtonEvent* a_event)
	{
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

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
		case kDeviceType_Gamepad:
			_platform = kPlatform_Other;
			Register(kScaleform_SetPlatform);
			Register(kScaleform_UpdateButtons);
			switch (a_event->keyMask) {
			case Gamepad::kGamepad_Up:
				ModSelectedIndex(-1);
				break;
			case Gamepad::kGamepad_Down:
				ModSelectedIndex(1);
				break;
			}
			break;
		case kDeviceType_Mouse:
			_platform = kPlatform_PC;
			Register(kScaleform_SetPlatform);
			Register(kScaleform_UpdateButtons);
			switch (a_event->keyMask) {
			case Mouse::kMouse_WheelUp:
				ModSelectedIndex(-1);
				break;
			case Mouse::kMouse_WheelDown:
				ModSelectedIndex(1);
				break;
			}
			break;
		case kDeviceType_Keyboard:
			_platform = kPlatform_PC;
			Register(kScaleform_SetPlatform);
			Register(kScaleform_UpdateButtons);
			if (controlID == strHolder->zoomIn) {
				ModSelectedIndex(-1);
			} else if (controlID == strHolder->zoomOut) {
				ModSelectedIndex(1);
			}
			break;
		}
		return true;
	}


	void LootMenu::OnMenuOpen()
	{
		typedef RE::BSGamepadDevice			BSGamepadDevice;
		typedef RE::BSWin32GamepadDevice	BSWin32GamepadDevice;

		if (!_containerRef) {
			return;
		}

		RE::BSGamepadDevice* gamepadHandle = RE::InputManager::GetSingleton()->GetGamepad();
		RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
		if (gamepad && gamepad->IsEnabled()) {
			_platform = kPlatform_Other;
		} else {
			_platform = kPlatform_PC;
		}

		_selectedIndex = 0;
		_skipInputCount = 0;
		_isMenuOpen = true;
		Register(kScaleform_SetKeyMappings);
		Register(kScaleform_SetPlatform);
		Register(kScaleform_SetContainer);
		Register(kScaleform_UpdateButtons);
		if (IsValidPickPocketTarget(_containerRef, RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
			Register(kScaleform_HideButtons);
		}
		Register(kScaleform_OpenContainer);
		Register(kScaleform_SetSelectedIndex);
		SetVisible(true);
		ProcessMessageQueue();
	}


	void LootMenu::OnMenuClose()
	{
		if (IsOpen()) {
			SetVisible(false);
			_isMenuOpen = false;
			Register(kScaleform_CloseContainer);
			PlayAnimationClose();
		}
	}


	void LootMenu::TakeItemStack()
	{
		if (!IsOpen() || !_containerRef || _displaySize <= 0) {
			return;
		}

		ItemData itemCopy(g_invList[_selectedIndex]);

		SInt32 numItems = itemCopy.count();
		if (numItems > 1 && SingleLootEnabled()) {
			numItems = 1;
		}

		if (TakeItem(itemCopy, numItems, true, true)) {
			DelayedUpdater::Register();
		}
	}


	void LootMenu::TakeAllItems()
	{
		if (!IsOpen() || !_containerRef || _displaySize <= 0) {
			return;
		}

		if (IsValidPickPocketTarget(_containerRef, RE::PlayerCharacter::GetSingleton()->IsSneaking())) {
			return;
		}

		_inTakeAllMode = true;

		UInt32 playSound = 5;
		for (auto& item : g_invList) {
			TakeItem(item, item.count(), false, playSound);
			if (playSound) {
				--playSound;
			}
		}
		g_invList.clear();
		SkipNextInput();

		_inTakeAllMode = false;
	}


	bool LootMenu::SingleLootEnabled()
	{
		typedef RE::BSKeyboardDevice		BSKeyboardDevice;
		typedef RE::BSWin32KeyboardDevice	BSWin32KeyboardDevice;
		typedef RE::BSGamepadDevice			BSGamepadDevice;
		typedef RE::BSWin32GamepadDevice	BSWin32GamepadDevice;
		typedef RE::InputEvent::DeviceType	DeviceType;

		if (Settings::disableSingleLoot) {
			return false;
		}

		RE::InputManager* inputManager = RE::InputManager::GetSingleton();
		RE::BSWin32KeyboardDevice* keyboard = DYNAMIC_CAST(inputManager->keyboard, BSKeyboardDevice, BSWin32KeyboardDevice);
		if (keyboard && keyboard->IsEnabled()) {
			UInt32 singleLootKeyboard = GetSingleLootKey(DeviceType::kDeviceType_Keyboard);
			if (singleLootKeyboard != RE::InputMappingManager::kInvalid && keyboard->IsPressed(singleLootKeyboard)) {
				return true;
			}
		}

		RE::BSGamepadDevice* gamepadHandle = 0;
		gamepadHandle = inputManager->GetGamepad();
		RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
		if (gamepad && gamepad->IsEnabled()) {
			UInt32 singleLootSprint = GetSingleLootKey(DeviceType::kDeviceType_Gamepad);
			if (singleLootSprint != RE::InputMappingManager::kInvalid && gamepad->IsPressed(singleLootSprint)) {
				return true;
			}
		}

		return false;
	}


	void LootMenu::PlayAnimation(const char* a_fromName, const char* a_toName)
	{
		typedef RE::NiControllerManager NiControllerManager;

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

		_containerRef->PlayAnimation(manager, toSeq, fromSeq, false);
	}


	void LootMenu::PlayAnimationOpen()
	{
		if (_containerRef && !_isContainerOpen) {
			PlayAnimation("Close", "Open");
			if (_containerRef->formType != RE::FormType::Character) {
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
		typedef RE::PlayerCharacter::EventType				EventType;
		typedef RE::TESObjectREFR::RemoveType				RemoveType;
		typedef RE::EffectSetting::Properties::Archetype	Archetype;

		bool manualUpdate = false;	// picking up dropped items doesn't disptach a container changed event

		// Locate item's extra list (if any)
		RE::BaseExtraList* xList = 0;
		if (a_item.entryData()->extraList && !a_item.entryData()->extraList->empty()) {
			xList = a_item.entryData()->extraList->front();
		}

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		// Pickup dropped items
		if (xList && xList->HasType(kExtraData_ItemDropper)) {
			RE::TESObjectREFR* refItem = reinterpret_cast<RE::TESObjectREFR*>((uintptr_t)xList - 0x70);
			player->PickUpItem(refItem, 1, false, true);
			manualUpdate = true;
		} else {
			RemoveType lootMode = RemoveType::kRemoveType_Take;

			if (_containerRef->baseForm->Is(RE::FormType::NPC)) {
				// Dead body
				if (_containerRef->IsDead(false)) {
					player->PlayPickupEvent(a_item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_DeadBody);
				// Pickpocket
				} else {
					if (!TryToPickPocket(a_item, lootMode)) {
						return manualUpdate;
					}
				}
			} else {
				// Container
				player->PlayPickupEvent(a_item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_Container);

				// Stealing
				if (_containerRef->IsOffLimits()) {
					lootMode = RemoveType::kRemoveType_Steal;
				}
			}

			// Remove projectile 3D
			RE::TESBoundObject* bound = static_cast<RE::TESBoundObject*>(a_item.form());
			if (bound) {
				bound->OnRemovedFrom(_containerRef);
			}

			if (_containerRef->baseForm->Is(RE::FormType::Character)) {
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
			player->DispellEffectsWithArchetype(Archetype::kArchetype_Invisibility, false);
			UInt32 droppedHandle = 0;
			_containerRef->RemoveItem(&droppedHandle, a_item.form(), a_numItems, lootMode, xList, player, 0, 0);
		}

		return manualUpdate;
	}


	bool LootMenu::TryToPickPocket(ItemData& a_item, RE::TESObjectREFR::RemoveType& a_lootMode)
	{
		using RE::_SendItemsPickPocketedEvent;

		typedef RE::PlayerCharacter::EventType	EventType;
		typedef RE::TESObjectREFR::RemoveType	RemoveType;

		RE::Actor* target = static_cast<RE::Actor*>(_containerRef);
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		bool pickSuccess = player->TryToPickPocket(target, a_item.entryData(), a_item.count(), true);
		player->PlayPickupEvent(a_item.entryData()->type, _containerRef->GetActorOwner(), _containerRef, EventType::kEventType_Thief);
		a_lootMode = RemoveType::kRemoveType_Steal;
		if (!pickSuccess) {
			return false;
		} else {
			_SendItemsPickPocketedEvent(a_item.count());
			return true;
		}
	}


	void LootMenu::DispellWornItemEnchantments()
	{
		RE::Actor* actor = static_cast<RE::Actor*>(_containerRef);
		if (actor->processManager) {
			actor->DispelWornItemEnchantments();
			actor->processManager->UpdateEquipment_Hooked(actor);
		}
	}


	UInt32 LootMenu::GetSingleLootKey(RE::InputEvent::DeviceType a_deviceType)
	{
		RE::BSFixedString str = _singleLootMapping.c_str();
		return RE::InputMappingManager::GetSingleton()->GetMappedKey(str, a_deviceType);
	}


	void LootMenu::ProcessMessageQueue()
	{
		using RE::_DebugNotification;

		const char* msg = 0;
		while (!_messageQueue.empty()) {
			msg = _messageQueue.front();
			_messageQueue.pop();
			_DebugNotification(msg, 0, true);
		}
	}


	LootMenu*				LootMenu::_singleton = 0;
	SInt32					LootMenu::_selectedIndex = 0;
	SInt32					LootMenu::_displaySize = 0;
	SInt32					LootMenu::_skipInputCount = 0;
	RE::TESObjectREFR*		LootMenu::_containerRef = 0;
	bool					LootMenu::_isContainerOpen = false;
	bool					LootMenu::_isMenuOpen = false;
	bool					LootMenu::_inTakeAllMode = false;
	bool					LootMenu::_isRegistered = false;
	bool					LootMenu::_isEnabled = true;
	LootMenu::Platform		LootMenu::_platform = kPlatform_PC;
	std::string				LootMenu::_actiText = "";
	std::string				LootMenu::_singleLootMapping = "";
	std::string				LootMenu::_takeMapping = "";
	std::string				LootMenu::_takeAllMapping = "";
	std::string				LootMenu::_searchMapping = "";
	std::queue<const char*>	LootMenu::_messageQueue;
}
