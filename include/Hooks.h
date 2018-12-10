#pragma once

#include "HookShare.h"  // _RegisterHook_t

#include "RE/BSFixedString.h"  // BSFixedString


namespace Hooks
{
	enum class ControlID
	{
		kNone,
		kActivate,
		kReadyWeapon,
		kTogglePOV,
		kJump,
		kSprint,
		kSneak,
		kShout,
		kToggleRun,
		kAutoMove,
		kFavorites
	};


	RE::BSFixedString& GetControlID(ControlID a_controlID);

	bool CheckForMappingConflicts();

	void InstallHooks(HookShare::_RegisterHook_t* a_register);


	static bool activateHandlerHooked = false;
	static bool cameraStateHandlerHooked = false;
}
