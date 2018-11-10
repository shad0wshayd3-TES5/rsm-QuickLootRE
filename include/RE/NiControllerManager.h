#pragma once

#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/NiControllers.h"  // NiTimeController

#include "Offsets.h"


namespace RE
{
	class NiControllerSequence;


	class NiControllerManager : public NiTimeController
	{
	public:
		NiControllerSequence* GetSequenceByName(const BSFixedString& name);

		// members
		NiTArray<NiControllerSequence*>	m_sequences;	// 50

	private:
		MEMBER_FN_PREFIX(NiControllerManager);
		DEFINE_MEMBER_FN(GetSequenceByName_Impl, NiControllerSequence*, NI_CONTROLLER_MANAGER_GET_SEQUENCE_BY_NAME_IMPL, const BSFixedString& name);
	};
}
