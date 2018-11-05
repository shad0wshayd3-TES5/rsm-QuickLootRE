#pragma once

#undef PlaySound

#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

class BGSSoundDescriptorForm;
class NiPoint3;

namespace RE
{
	class NiControllerManager;
	class NiControllerSequence;
	class NiNode;
	class TESObjectREFR;
}


namespace Hooks
{
	typedef void _PlayAnimation(RE::TESObjectREFR* a_refr, RE::NiControllerManager* a_manager, RE::NiControllerSequence* a_toSeq, RE::NiControllerSequence* a_fromSeq, bool a_unk);
	extern RelocAddr<_PlayAnimation*> PlayAnimation;

	typedef bool _PlaySound(BGSSoundDescriptorForm* a_sound, UInt32 a_flag, const NiPoint3* a_position, RE::NiNode* a_unk);
	extern RelocAddr<_PlaySound*> PlaySound;

	typedef void _SendItemsPickPocketedEvent(UInt32 numItems);
	extern RelocAddr<_SendItemsPickPocketedEvent*> SendItemsPickPocketedEvent;

	void installHooks();
}
