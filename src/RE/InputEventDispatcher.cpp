#include "RE/InputEventDispatcher.h"

#include "skse64/GameInput.h"  // InputEventDispatcher

#include "RE/BSGamepadDevice.h"  // BSGamepadDevice
#include "RE/BSPCGamepadDeviceHandler.h"  // BSPCGamepadDeviceHandler


namespace RE
{
	InputEventDispatcher* InputEventDispatcher::GetSingleton()
	{
		return reinterpret_cast<InputEventDispatcher*>(::InputEventDispatcher::GetSingleton());
	}


	bool InputEventDispatcher::IsGamepadEnabled()
	{
		return gamepadHandler && gamepadHandler->gamepad && gamepadHandler->gamepad->IsEnabled();
	}


	BSGamepadDevice* InputEventDispatcher::GetGamepad()
	{
		return gamepadHandler ? gamepadHandler->gamepad : 0;
	}
}
