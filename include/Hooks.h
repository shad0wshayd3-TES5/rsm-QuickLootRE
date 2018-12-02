#pragma once

#include "HookShare.h"  // _RegisterHook_t

#include "RE/BSFixedString.h"  // BSFixedString


namespace Hooks
{
	enum ControlID
	{
		kControlID_None,
		kControlID_Activate,
		kControlID_ReadyWeapon,
		kControlID_TogglePOV,
		kControlID_Jump,
		kControlID_Sprint,
		kControlID_Sneak,
		kControlID_Shout,
		kControlID_ToggleRun,
		kControlID_AutoMove
	};


	RE::BSFixedString& GetControlID(ControlID a_controlID);

	bool CheckForMappingConflicts();

	void InstallHooks(HookShare::_RegisterHook_t* a_register);


	static bool activateHandlerHooked = false;
	static bool cameraStateHandlerHooked = false;
}
