#include "RE/BaseExtraList.h"


namespace RE
{
	bool BaseExtraList::HasType(UInt32 type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->HasType(type);
	}


	void BaseExtraList::MarkType(UInt32 type, bool bCleared)
	{
		reinterpret_cast<::BaseExtraList*>(this)->MarkType(type, bCleared);
	}


	bool BaseExtraList::Remove(UInt8 type, BSExtraData* toRemove)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Remove(type, toRemove);
	}


	bool BaseExtraList::Add(UInt8 type, BSExtraData* toAdd)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Add(type, toAdd);
	}


	bool BaseExtraList::CheckContainerExtraData(bool isEquipped)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->CheckContainerExtraData(isEquipped);
	}


	const char* BaseExtraList::GetDisplayName(TESForm* type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetDisplayName(type);
	}


	BSExtraData* BaseExtraList::GetByType(UInt32 type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetByType(type);
	}


	void BaseExtraList::SetInventoryChanges(ExtraContainerChanges::Data* changes)
	{
		CALL_MEMBER_FN(this, SetInventoryChanges_Impl)(changes);
	}


	UInt32 BaseExtraList::GetAshPileRefHandle(UInt32& refHandle)
	{
		return CALL_MEMBER_FN(this, GetAshPileRefHandle_Impl)(refHandle);
	}
}
