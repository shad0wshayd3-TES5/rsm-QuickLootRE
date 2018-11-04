#include "LootMenu.h"

#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameSettings.h"  // g_gameSettingCollection
#include "skse64/GameInput.h"  // InputEvent, InputStringHolder
#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/NiRTTI.h"  // ni_cast
#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include "Delegates.h"
#include "HasActivateChoiceVisitor.h"  // HasActivateChoiceVisitor
#include "Hooks.h"  // PlayAnimation(), PlaySound(), SendItemsPickPocketedEvent()
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList
#include "Settings.h"  // Settings

#include "RE/ActorProcessManager.h"  // ActorProcessManager
#include "RE/BGSEntryPointPerkEntry.h"  // BGSEntryPointPerkEntry
#include "RE/BSWin32GamepadDevice.h"  // BSWin32GamepadDevice
#include "RE/BSWin32KeyboardDevice.h"  // BSWin32KeyboardDevice
#include "RE/BSWin32MouseDevice.h"  // BSWin32MouseDevice
#include "RE/ButtonEvent.h"  // ButtonEvent
#include "RE/ExtraContainerChanges.h"  // ExtraContainerChanges, ExtraContainerChanges::Data
#include "RE/GFxMovieDef.h"  // GFxMovieDef
#include "RE/GFxMovieView.h"  // GFxMovieView
#include "RE/GFxLoader.h"  // GFxLoader
#include "RE/IMenu.h"  // IMenu
#include "RE/InputEventDispatcher.h"  // InputEventDispatcher
#include "RE/InputManager.h"  // InputMappingManager
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/MenuControls.h"  // MenuControls
#include "RE/MenuManager.h"  // MenuManager
#include "RE/NiControllerManager.h"  // NiControllerManager
#include "RE/NiNode.h"  // NiNode
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESBoundObject.h"  // TESBoundObject
#include "RE/TESObjectREFR.h"  // TESObjectREFR

class TESObjectREFR;


namespace QuickLootRE
{
	RE::IMenu* LootMenuCreator::Create()
	{
		void* p = ScaleformHeap_Allocate(sizeof(LootMenu));
		if (p) {
			LootMenu::_singleton = new (p) LootMenu(LootMenu::GetName());
			return LootMenu::_singleton;
		} else {
			return 0;
		}
	}


	LootMenu::LootMenu(const char* a_swfPath)
	{
		typedef RE::GFxMovieView::ScaleModeType ScaleModeType;

		RE::GFxLoader* loader = RE::GFxLoader::GetSingleton();
		if (loader->LoadMovie(this, view, a_swfPath, ScaleModeType::kScaleModeType_ShowAll, 0.0)) {
			flags = kFlag_DoNotDeleteOnClose | kFlag_DoNotPreventGameSave | kFlag_Unk10000;
			context = 0x02;  // menuDepth, set lower than fade menu (3)
		}
	}


	LootMenu::~LootMenu()
	{
		if (this) {
			ScaleformHeap_Free(this);
		}
	}


	LootMenu* LootMenu::GetSingleton()
	{
		return _singleton;
	}


	BSFixedString LootMenu::GetName()
	{
		static BSFixedString name = "LootMenu";
		return name;
	}


	void LootMenu::SetContainerRef(TESObjectREFR* a_ref)
	{
		_containerRef = reinterpret_cast<RE::TESObjectREFR*>(a_ref);
	}


	void LootMenu::SetContainerRef(RE::TESObjectREFR* a_ref)
	{
		_containerRef = a_ref;
	}


	RE::TESObjectREFR* LootMenu::GetContainerRef()
	{
		return _containerRef;
	}


	void LootMenu::ClearContainerRef(bool a_playAnimation)
	{
		if (a_playAnimation && IsOpen()) {
			_singleton->PlayAnimationClose();
		}
		_containerRef = 0;
	}


	bool LootMenu::CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking)
	{
		static RE::MenuManager*		mm						= RE::MenuManager::GetSingleton();
		static RE::InputManager*	mapping					= RE::InputManager::GetSingleton();
		static RE::PlayerCharacter*	player					= reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);
		static BSFixedString		strAnimationDriven		= "bAnimationDriven";
		static TESFaction*			CurrentFollowerFaction	= static_cast<TESFaction*>(LookupFormByID(0x0005C84E));

		if (!a_ref || !a_ref->baseForm) {
			return false;
		}

		if (mm && mm->numPauseGame && mm->numStopCrosshairUpdate > 0) {
			return false;
		}


		if (!mapping || !mapping->IsMovementControlsEnabled()) {
			return false;
		}


		if (player->GetGrabbedRef() || player->GetActorInFavorState() || player->IsInKillMove()) {
			return false;
		}


		bool bAnimationDriven;
		if (player->animGraphHolder.GetAnimationVariableBool(strAnimationDriven, bAnimationDriven) && bAnimationDriven) {
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
		case kFormType_Activator:
		{
			UInt32 refHandle = a_ref->extraData.GetAshPileRefHandle(refHandle);
			if (refHandle != *g_invalidRefHandle) {
				RE::TESObjectREFR* refPtr = 0;
				if (RE::TESObjectREFR::LookupByHandle(refHandle, refPtr)) {
					containerRef = refPtr;
				}
			}
			break;
		}
		case kFormType_Container:
			if (!a_ref->IsLocked()) {
				containerRef = a_ref;
			}
			break;
		case kFormType_NPC:
			if (a_ref->IsDead(true)) {
				containerRef = a_ref;
			} else if (!Settings::disablePickPocketing && !a_ref->IsChild() && a_isSneaking) {
				RE::Actor* target = static_cast<RE::Actor*>(a_ref);
				if (!target->IsPlayerTeammate() && !target->IsInFaction(CurrentFollowerFaction)) {
					if (!target->IsInCombat()) {
						containerRef = a_ref;
					}
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

#if 0
		// Disabled until I can understand this better
		if (player->CanProcessEntryPointPerkEntry(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate)) {
			HasActivateChoiceVisitor visitor(player, a_ref);
			player->VisitEntryPointPerkEntries(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate, visitor);
			if (visitor.GetResult()) {
				return false;
			}
		}
#endif

		_containerRef = containerRef;

		return true;
	}


	bool LootMenu::IsOpen()
	{
		return _isOpen;
	}


	void LootMenu::Register(Scaleform a_reg)
	{
		switch (a_reg) {
		case kScaleform_SetPlatform:
		{
			SetPlatforUIDelegate* dlgt = (SetPlatforUIDelegate*)Heap_Allocate(sizeof(SetPlatforUIDelegate));
			new (dlgt)SetPlatforUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_Setup:
		{
			SetupUIDelegate* dlgt = (SetupUIDelegate*)Heap_Allocate(sizeof(SetupUIDelegate));
			new (dlgt)SetupUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_OpenContainer:
		{
			OpenContainerUIDelegate* dlgt = (OpenContainerUIDelegate*)Heap_Allocate(sizeof(OpenContainerUIDelegate));
			new (dlgt)OpenContainerUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_SetContainer:
		{
			SetContainerUIDelegate* dlgt = (SetContainerUIDelegate*)Heap_Allocate(sizeof(SetContainerUIDelegate));
			new (dlgt)SetContainerUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_CloseContainer:
		{
			CloseContainerUIDelegate* dlgt = (CloseContainerUIDelegate*)Heap_Allocate(sizeof(CloseContainerUIDelegate));
			new (dlgt)CloseContainerUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_SetSelectedIndex:
		{
			SetSelectedIndexUIDelegate* dlgt = (SetSelectedIndexUIDelegate*)Heap_Allocate(sizeof(SetSelectedIndexUIDelegate));
			new (dlgt)SetSelectedIndexUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		default:
			_ERROR("[ERROR] Invalid registration (%i)", a_reg);
		}
	}


	UInt32 LootMenu::ProcessMessage(UIMessage* a_message)
	{
		if (!Settings::isApplied) {
			Register(kScaleform_Setup);
		}

		switch (a_message->message) {
		case UIMessage::kMessage_Open:
			OnMenuOpen();
			break;
		case UIMessage::kMessage_Close:
			OnMenuClose();
			break;
		}
		return 2;
	}


	void LootMenu::Render()
	{
		if (IsOpen()) {
			view->Render();
		}
	}


	bool LootMenu::CanProcess(InputEvent* a_event)
	{
		typedef RE::BSInputDevice::InputDevice		InputDevice;
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

		static InputStringHolder* strHolder = InputStringHolder::GetSingleton();

		if (IsOpen() && a_event->eventType == InputEvent::kEventType_Button) {
			ButtonEvent* button = static_cast<ButtonEvent*>(a_event);

			BSFixedString controlID = *a_event->GetControlID();

			if (controlID == strHolder->sneak ||
				controlID == strHolder->pause ||
				controlID == strHolder->journal) {
				return true;
			}

			switch (a_event->deviceType) {
			case InputDevice::kInputDevice_Gamepad:
				return (button->keyMask == Gamepad::kGamepad_Up || button->keyMask == Gamepad::kGamepad_Down);
			case InputDevice::kInputDevice_Mouse:
				return (button->keyMask == Mouse::kMouse_WheelDown || button->keyMask == Mouse::kMouse_WheelUp);
			case InputDevice::kInputDevice_Keyboard:
				return (controlID == strHolder->zoomIn || controlID == strHolder->zoomOut);
			}
		}
		return false;
	}


	bool LootMenu::ProcessButton(RE::ButtonEvent* a_event)
	{
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

		static InputStringHolder*	strHolder	= InputStringHolder::GetSingleton();
		static UIManager*			uiManager	= UIManager::GetSingleton();
		static RE::PlayerCharacter*	player		= reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		if (!a_event->IsDown()) {
			return true;
		}

		BSFixedString controlID = *a_event->GetControlID();
		if (controlID == strHolder->sneak) {
			CALL_MEMBER_FN(uiManager, AddMessage)(&GetName(), UIMessage::kMessage_Close, 0);
			if (CanOpen(_containerRef, !player->IsSneaking())) {
				CALL_MEMBER_FN(uiManager, AddMessage)(&GetName(), UIMessage::kMessage_Open, 0);
			}
			return true;
		} else if (controlID == strHolder->pause ||
				   controlID == strHolder->journal) {
			CALL_MEMBER_FN(uiManager, AddMessage)(&GetName(), UIMessage::kMessage_Close, 0);
		}

		switch (a_event->deviceType) {
		case kDeviceType_Gamepad:
			if (_platform != kPlatform_Other) {
				_platform = kPlatform_Other;
				Register(kScaleform_SetPlatform);
			}
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
			if (_platform != kPlatform_PC) {
				_platform = kPlatform_PC;
				Register(kScaleform_SetPlatform);
			}
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
		{
			if (_platform != kPlatform_PC) {
				_platform = kPlatform_PC;
				Register(kScaleform_SetPlatform);
			}
			if (controlID == strHolder->zoomIn) {
				ModSelectedIndex(-1);
			} else if (controlID == strHolder->zoomOut) {
				ModSelectedIndex(1);
			}
			break;
		}
		}
		return true;
	}


	void LootMenu::OnMenuOpen()
	{
		_selectedIndex = 0;
		if (!IsOpen()) {
			PlayAnimationOpen();
		}
		_isOpen = true;
		Register(kScaleform_SetContainer);
		Register(kScaleform_OpenContainer);
		Register(kScaleform_SetSelectedIndex);
	}


	void LootMenu::OnMenuClose()
	{
		if (IsOpen()) {
			_isOpen = false;
			Register(kScaleform_CloseContainer);
		}
	}


	void LootMenu::TakeItem()
	{
		typedef RE::PlayerCharacter::EventType	EventType;
		typedef RE::TESObjectREFR::RemoveType	RemoveType;

		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		if (!IsOpen() || !_containerRef || g_invList.empty()) {
			return;
		}

		// Evaluate # of items to remove and update inv list accordingly
		ItemData item = g_invList[_selectedIndex];
		SInt32 numItems = item.count();
		if (numItems > 1 && SingleLootEnabled()) {
			numItems = 1;
			g_invList[_selectedIndex].reduceCount();
		} else {
			g_invList.erase(g_invList.begin() + _selectedIndex);
			ModSelectedIndex(0);
		}

		// Containers don't have ExtraContainerChanges if the player hasn't opened them yet, so we must add them ourselves
		RE::ExtraContainerChanges* xContainerChanges = static_cast<RE::ExtraContainerChanges*>(_containerRef->extraData.GetByType(kExtraData_ContainerChanges));
		RE::ExtraContainerChanges::Data* changes = xContainerChanges ? xContainerChanges->data : 0;
		if (!changes) {
			RE::ExtraContainerChanges::Data* changes = (RE::ExtraContainerChanges::Data*)Heap_Allocate(sizeof(RE::ExtraContainerChanges::Data));
			new (changes)RE::ExtraContainerChanges::Data(_containerRef);
			_containerRef->extraData.SetInventoryChanges(changes);
			changes->InitContainer();
		}

		// Locate item's extra list (if any)
		BaseExtraList* xList = 0;
		if (item.entryData()->extendDataList && item.entryData()->extendDataList->Count() > 0) {
			xList = item.entryData()->extendDataList->GetNthItem(0);
		}

		// Pickup dropped items
		if (xList && xList->HasType(kExtraData_ItemDropper)) {
			RE::TESObjectREFR* refItem = reinterpret_cast<RE::TESObjectREFR*>((UInt64)xList - 0x70);
			player->PickUpItem(refItem, 1, false, true);
		} else {
			RemoveType lootMode = RemoveType::kRemoveType_Take;

			if (_containerRef->baseForm->formType == kFormType_NPC) {
				if (_containerRef->IsDead(false)) {
					player->PlayPickupEvent(item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_DeadBody);
				} else {
					RE::Actor* target = static_cast<RE::Actor*>(_containerRef);
					bool pickSuccess = player->TryToPickPocket(target, item.entryData(), item.count(), true);
					player->PlayPickupEvent(item.entryData()->type, _containerRef->GetActorOwner(), _containerRef, EventType::kEventType_Thief);
					lootMode = RemoveType::kRemoveType_Steal;
					if (!pickSuccess) {
						return;
					} else {
						(*Hooks::SendItemsPickPocketedEvent)(item.count());
					}
				}
			} else {
				player->PlayPickupEvent(item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_Container);

				if (_containerRef->IsOffLimits()) {
					lootMode = RemoveType::kRemoveType_Steal;
				}
			}

			// Remove projectile 3D
			RE::TESBoundObject* bound = static_cast<RE::TESBoundObject*>(item.form());
			if (bound) {
				bound->OnRemovedFrom(_containerRef);
			}

			if (_containerRef->baseForm->formType == kFormType_Character) {

				// Dispell worn item enchantments
				RE::Actor* actor = static_cast<RE::Actor*>(_containerRef);
				if (actor->processManager) {
					actor->DispelWornItemEnchantments();
					actor->processManager->UpdateEquipment_Hooked(actor);
				}
			} else {
				if (_containerRef->IsOffLimits()) {
					player->SendStealAlarm(_containerRef, item.entryData()->type, numItems, item.value(), _containerRef->GetOwner(), true);
				}
			}

			player->PlaySounds(item.form(), true, false);

			UInt32 droppedHandle = 0;
			_containerRef->RemoveItem(&droppedHandle, item.form(), numItems, lootMode, xList, player, 0, 0);
		}

		//Register(kScaleform_OpenContainer);
	}


	void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
	{
		if (IsOpen()) {
			_selectedIndex += a_indexOffset;
			SInt32 maxItemIdx = g_invList.size() < Settings::itemLimit ? g_invList.size() - 1 : Settings::itemLimit - 1;
			if (_selectedIndex < 0) {
				_selectedIndex = 0;
			} else if (_selectedIndex > maxItemIdx) {
				_selectedIndex = maxItemIdx;
			}
			Register(kScaleform_SetSelectedIndex);
		}
	}


	bool LootMenu::SingleLootEnabled()
	{
		typedef RE::BSKeyboardDevice			BSKeyboardDevice;
		typedef RE::BSWin32KeyboardDevice		BSWin32KeyboardDevice;
		typedef RE::BSGamepadDevice				BSGamepadDevice;
		typedef RE::BSWin32GamepadDevice		BSWin32GamepadDevice;
		typedef RE::BSInputDevice::InputDevice	InputDevice;

		static RE::InputEventDispatcher*	inputDispatcher	= RE::InputEventDispatcher::GetSingleton();
		static RE::InputManager*			inputManager	= RE::InputManager::GetSingleton();
		static InputStringHolder*			holder			= InputStringHolder::GetSingleton();
		static UInt32						keyRun			= inputManager->GetMappedKey(holder->sprint, InputDevice::kInputDevice_Keyboard);
		static UInt32						keySprint		= inputManager->GetMappedKey(holder->sprint, InputDevice::kInputDevice_Gamepad);

		if (Settings::disableSingleLoot) {
			return false;
		}

		RE::BSWin32KeyboardDevice* keyboard = DYNAMIC_CAST(inputDispatcher->keyboard, BSKeyboardDevice, BSWin32KeyboardDevice);
		if (keyboard && keyboard->IsEnabled()) {
			if (keyRun != RE::InputManager::kInvalid && keyboard->IsPressed(keyRun)) {
				return true;
			}
		}

		RE::BSGamepadDevice* gamepadHandle = inputDispatcher->gamepadHandler ? inputDispatcher->gamepadHandler->gamepad : 0;
		RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
		if (gamepad && gamepad->IsEnabled()) {
			if (keySprint != RE::InputManager::kInvalid && gamepad->IsPressed(keySprint)) {
				return true;
			}
		}

		return false;
	}


	void LootMenu::PlayAnimation(const char* fromName, const char* toName)
	{
		if (Settings::playAnimations) {
			typedef RE::NiControllerManager NiControllerManager;

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

			RE::NiControllerSequence* fromSeq = manager->GetSequenceByName(fromName);
			RE::NiControllerSequence* toSeq = manager->GetSequenceByName(toName);
			if (!fromSeq || !toSeq) {
				return;
			}

			(*Hooks::PlayAnimation)(_containerRef, manager, toSeq, fromSeq, false);
		}
	}


	void LootMenu::PlayAnimationOpen()
	{
		if (_containerRef) {
			PlayAnimation("Close", "Open");
			if (_containerRef->formType != kFormType_Character) {
				_containerRef->ActivateRefChildren(*g_thePlayer);
			}
		}
	}


	void LootMenu::PlayAnimationClose()
	{
		if (_containerRef) {
			PlayAnimation("Open", "Close");
		}
	}


	void LootMenu::PlaySound(TESForm* a_item)
	{
		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		BGSPickupPutdownSounds* sounds = DYNAMIC_CAST(a_item, TESForm, BGSPickupPutdownSounds);
		if (sounds && sounds->pickUp) {
			(*Hooks::PlaySound)(sounds->pickUp, false, &_containerRef->pos, player->GetNiNode());
		}
	}


	LootMenu*			LootMenu::_singleton = 0;
	SInt32				LootMenu::_selectedIndex = 0;
	RE::TESObjectREFR*	LootMenu::_containerRef = 0;
	bool				LootMenu::_isOpen = false;
	LootMenu::Platform	LootMenu::_platform = kPlatform_PC;
}
