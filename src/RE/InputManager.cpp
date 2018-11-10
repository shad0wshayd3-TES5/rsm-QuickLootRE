#include "RE/InputManager.h"

#include "skse64/GameTypes.h"  // BSFixedString, tArray
#include "skse64/GameInput.h"  // InputManager


namespace RE
{
	InputManager* InputManager::GetSingleton()
	{
		return reinterpret_cast<InputManager*>(::InputManager::GetSingleton());
	}


	UInt8 InputManager::AllowTextInput(bool allow)
	{
		return reinterpret_cast<::InputManager*>(this)->AllowTextInput(allow);
	}


	UInt32 InputManager::GetMappedKey(const BSFixedString& name, InputDevice deviceType, Context contextIdx) const
	{
		tArray<InputContext::Mapping>* maps = 0;

		switch (deviceType) {
		case InputDevice::kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case InputDevice::kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case InputDevice::kInputDevice_Keyboard:
			maps = &context[contextIdx]->keyboardMap;
			break;
		}

		if (maps) {
			InputContext::Mapping mapping;
			for (int i = 0; i < maps->count; ++i) {
				if (maps->GetNthItem(i, mapping) && mapping.name == name) {
					return mapping.buttonID;
				}
			}
		}

		return kInvalid;
	}


	const BSFixedString& InputManager::GetUserEventName(UInt32 buttonID, InputDevice deviceType, Context contextIdx) const
	{
		tArray<InputContext::Mapping>* maps = 0;

		switch (deviceType) {
		case InputDevice::kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case InputDevice::kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case InputDevice::kInputDevice_Keyboard:
			maps = &context[contextIdx]->keyboardMap;
			break;
		}

		static BSFixedString none = "";

		if (maps) {
			static InputContext::Mapping mapping;
			for (int i = 0; i < maps->count; ++i) {
				if (maps->GetNthItem(i, mapping) && mapping.buttonID == buttonID) {
					return mapping.name;
				}
			}
		}

		return none;
	}


	bool InputManager::IsLookingControlsEnabled() const
	{
		return (controlState & kControlState_Looking) == kControlState_Looking;
	}


	bool InputManager::IsFlyingControlsEnabled() const
	{
		return (controlState & kControlState_Flying) == kControlState_Flying;
	}


	bool InputManager::IsSneakingControlsEnabled() const
	{
		return (controlState & kControlState_Sneaking) == kControlState_Sneaking;
	}


	bool InputManager::IsMenuControlsEnabled() const
	{
		return (controlState & kControlState_Menu) == kControlState_Menu;
	}


	bool InputManager::IsMovementControlsEnabled() const
	{
		return (controlState & kControlState_Movement) == kControlState_Movement;
	}
}
