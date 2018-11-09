#include "RE/PlayerCharacter.h"

#include "common/ITypes.h"  // UInt32

#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


namespace RE
{
	TESObjectREFR* PlayerCharacter::GetGrabbedRef()
	{
		TESObjectREFRPtr refPtr = 0;
		UInt32 handle = playerGrabbedHandle;
		if (handle != *g_invalidRefHandle) {
			TESObjectREFR::LookupByHandle(handle, refPtr);
		}
		return refPtr;
	}
}
