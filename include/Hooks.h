#pragma once

#undef PlaySound

#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

class TESForm;

namespace RE
{
	class Actor;
}


namespace Hooks
{
	typedef void _SendItemsPickPocketedEvent_t(UInt32 a_numItems);
	extern RelocAddr<_SendItemsPickPocketedEvent_t*> _SendItemsPickPocketedEvent;

	typedef UInt32 _GetPickPocketChance_t(float a_playerSkill, float a_targetSkill, UInt32 a_totalValue, float a_totalWeight, RE::Actor* a_player, RE::Actor* a_target, bool a_isDetected, TESForm* a_item);
	extern RelocAddr<_GetPickPocketChance_t*> _GetPickPocketChance;

	typedef void* _HeapAllocAbstraction_t(std::size_t a_allocSize);
	extern RelocAddr<_HeapAllocAbstraction_t*> _HeapAllocAbstraction;


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
