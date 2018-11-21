#pragma once

#include "skse64/GameTypes.h"  // BSFixedString


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
		kControlID_ToggleRun
	};


	BSFixedString& GetControlID(ControlID a_controlID);

	bool CheckForMappingConflicts();

	void InstallHooks();


	static bool activateHandlerHooked = false;
}
