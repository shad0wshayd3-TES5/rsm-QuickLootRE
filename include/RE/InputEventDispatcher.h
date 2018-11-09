#pragma once

#include "skse64/GameEvents.h"  // BSTEventSource
#include "skse64/GameInput.h"  // InputEventDispatcher

#include "RE/BSGamepadDevice.h"  // RE::BSGamepadDevice
#include "RE/BSPCGamepadDeviceHandler.h"  // RE::BSPCGamepadDeviceHandler

class InputEvent;


namespace RE
{
	class BSKeyboardDevice;
	class BSMouseDevice;
	class BSVirtualKeyboardDevice;


	class InputEventDispatcher
	{
	public:
		// parents
		void*								unk00;			// 00 - singleton
		BSTEventSource<InputEvent*>			unk08;			// 08


		static inline InputEventDispatcher*	GetSingleton()		{ return reinterpret_cast<InputEventDispatcher*>(::InputEventDispatcher::GetSingleton()); }

		inline bool							IsGamepadEnabled()	{ return gamepadHandler && gamepadHandler->gamepad && gamepadHandler->gamepad->IsEnabled(); }
		inline BSGamepadDevice*				GetGamepad()		{ return gamepadHandler ? gamepadHandler->gamepad : 0; }


		// members
		BSKeyboardDevice*					keyboard;		// 60 - BSWin32KeyboardDevice
		BSMouseDevice*						mouse;			// 68 - BSWin32MouseDevice
		BSPCGamepadDeviceHandler*			gamepadHandler;	// 70
		BSVirtualKeyboardDevice*			vitualKeyboard;	// 78 - BSWin32VirtualKeyboardDevice - New in SE
		bool								unk78;			// 80 - init'd true
		bool								unk79;			// 81 - init'd false
		UInt8								pad7A;			// 82
		UInt8								pad7B;			// 83
		UInt32								pad7C;			// 84
	};
	STATIC_ASSERT(offsetof(InputEventDispatcher, keyboard) == 0x60);
}
