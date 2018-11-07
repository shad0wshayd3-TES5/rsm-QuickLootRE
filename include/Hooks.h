#pragma once

#undef PlaySound

#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

class BGSSoundDescriptorForm;
class NiPoint3;

namespace RE
{
	class Actor;
	class InventoryEntryData;
	class NiControllerManager;
	class NiControllerSequence;
	class NiNode;
	class TESObjectREFR;
}


namespace Hooks
{
	typedef void _SendItemsPickPocketedEvent(UInt32 numItems);
	extern RelocAddr<_SendItemsPickPocketedEvent*> SendItemsPickPocketedEvent;

	void installHooks();
}
