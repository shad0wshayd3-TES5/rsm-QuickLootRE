#include "RE/TESObjectBOOK.h"


namespace RE
{
	UInt32 TESObjectBOOK::Data::GetSanitizedType()
	{
		return reinterpret_cast<::TESObjectBOOK*>(this)->data.GetSanitizedType();
	}


	bool TESObjectBOOK::IsRead() const
	{
		return (data.flags & Data::kType_Read) != 0;
	}
}
