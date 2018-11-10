#include "RE/NiControllerManager.h"

#include "skse64/GameTypes.h"  // BSFixedString


namespace RE
{
	NiControllerSequence* NiControllerManager::GetSequenceByName(const BSFixedString& name)
	{
		return CALL_MEMBER_FN(this, GetSequenceByName_Impl)(name);
	}
}
