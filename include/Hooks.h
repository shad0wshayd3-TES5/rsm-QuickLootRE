#pragma once

#include "HookShare.h"  // RegisterForCanProcess_t


namespace
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


	bool g_activateHandlerHooked = false;
	bool g_cameraStateHandlerHooked = false;
}


namespace Hooks
{
	void InstallHooks(HookShare::RegisterForCanProcess_t* a_register);
}
