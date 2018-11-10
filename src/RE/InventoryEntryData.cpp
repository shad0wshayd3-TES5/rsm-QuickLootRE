#include "RE/InventoryEntryData.h"


namespace RE
{
	InventoryEntryData* InventoryEntryData::Create(TESForm* item, UInt32 count)
	{
		return reinterpret_cast<InventoryEntryData*>(::InventoryEntryData::Create(item, count));
	}


	void InventoryEntryData::Delete(void)
	{
		reinterpret_cast<::InventoryEntryData*>(this)->Delete();
	}


	void InventoryEntryData::GetExtraWornBaseLists(BaseExtraList** pWornBaseListOut, BaseExtraList** pWornLeftBaseListOut)
	{
		return reinterpret_cast<::InventoryEntryData*>(this)->GetExtraWornBaseLists(reinterpret_cast<::BaseExtraList**>(pWornBaseListOut), reinterpret_cast<::BaseExtraList**>(pWornLeftBaseListOut));
	}


	void InventoryEntryData::GetEquipItemData(::InventoryEntryData::EquipData& stateOut, SInt32 itemId, SInt32 baseCount)
	{
		reinterpret_cast<::InventoryEntryData*>(this)->GetEquipItemData(stateOut, itemId, baseCount);
	}


	const char* InventoryEntryData::GenerateName()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GenerateName)();
	}


	SInt32 InventoryEntryData::GetValue()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GetValue)();
	}


	bool InventoryEntryData::IsOwnedBy(TESForm* actor, bool unk1)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), IsOwnedBy)(actor, unk1);
	}


	UInt32 InventoryEntryData::GetSoulLevel()
	{
		return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GetSoulLevel)();
	}


	bool InventoryEntryData::IsOwnedBy(TESForm* actor, TESForm* itemOwner, bool unk1)
	{
		return CALL_MEMBER_FN(this, IsOwnedBy)(actor, itemOwner, unk1);
	}


	TESForm* InventoryEntryData::GetOwner()
	{
		return CALL_MEMBER_FN(this, GetOwner)();
	}


	float InventoryEntryData::GetWeight()
	{
		return CALL_MEMBER_FN(this, GetWeight)();
	}
}
