#include "RE/NiObjectNET.h"


namespace RE
{
	void NiObjectNET::AddExtraData(NiExtraData* extraData)
	{
		reinterpret_cast<::NiObjectNET*>(this)->AddExtraData(extraData);
	}


	bool NiObjectNET::RemoveExtraData(NiExtraData* extraData)
	{
		return reinterpret_cast<::NiObjectNET*>(this)->RemoveExtraData(extraData);
	}


	SInt32 NiObjectNET::GetIndexOf(NiExtraData* extraData)
	{
		return reinterpret_cast<::NiObjectNET*>(this)->GetIndexOf(extraData);
	}


	NiExtraData* NiObjectNET::GetExtraData(BSFixedString name)
	{
		return reinterpret_cast<::NiObjectNET*>(this)->GetExtraData(name);
	}


	NiTimeController* NiObjectNET::GetController() const
	{
		return m_controller;
	}
}
