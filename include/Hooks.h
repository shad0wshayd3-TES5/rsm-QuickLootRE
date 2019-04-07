#pragma once

#include "HookShare.h"  // RegisterForCanProcess_t

#include "RE/Skyrim.h"


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


	// TEMPORARY
	RE::TESObjectREFRPtr g_crosshairRef;
	// TEMPORARY


	bool g_activateHandlerHooked = false;
	bool g_cameraStateHandlerHooked = false;
}


namespace Hooks
{
	// TEMPORARY
	bool Register_GetCurrentCrosshairRef_Hook(RE::BSScript::Internal::VirtualMachine* a_vm);
	// TEMPORARY

	void InstallHooks(HookShare::RegisterForCanProcess_t* a_register);
}
