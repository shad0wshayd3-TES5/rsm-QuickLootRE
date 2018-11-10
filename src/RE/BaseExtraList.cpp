#include "RE/BaseExtraList.h"


namespace RE
{
	bool BaseExtraList::HasType(UInt32 a_type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->HasType(a_type);
	}


	void BaseExtraList::MarkType(UInt32 a_type, bool a_bCleared)
	{
		reinterpret_cast<::BaseExtraList*>(this)->MarkType(a_type, a_bCleared);
	}


	bool BaseExtraList::Remove(UInt8 a_type, BSExtraData* a_toRemove)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Remove(a_type, a_toRemove);
	}


	bool BaseExtraList::Add(UInt8 a_type, BSExtraData* a_toAdd)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Add(a_type, a_toAdd);
	}


	bool BaseExtraList::CheckContainerExtraData(bool a_isEquipped)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->CheckContainerExtraData(a_isEquipped);
	}


	const char* BaseExtraList::GetDisplayName(TESForm* a_type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetDisplayName(a_type);
	}


	BSExtraData* BaseExtraList::GetByType(UInt32 a_type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetByType(a_type);
	}


	void BaseExtraList::SetInventoryChanges(ExtraContainerChanges::Data* a_changes)
	{
		_SetInventoryChanges_Impl(this, a_changes);
	}


	UInt32 BaseExtraList::GetAshPileRefHandle(UInt32& a_refHandle)
	{
		return _GetAshPileRefHandle_Impl(this, a_refHandle);
	}


	RelocAddr<BaseExtraList::_SetInventoryChanges_Impl_t*> BaseExtraList::_SetInventoryChanges_Impl(BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL);
	RelocAddr<BaseExtraList::_GetAshPileRefHandle_Impl_t*> BaseExtraList::_GetAshPileRefHandle_Impl(BASE_EXTRA_LIST_GET_ASH_PILE_REF_HANDLE_IMPL);
}
