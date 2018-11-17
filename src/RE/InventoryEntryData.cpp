#include "RE/InventoryEntryData.h"


namespace RE
{
	InventoryEntryData* InventoryEntryData::Create(TESForm* a_item, UInt32 a_count)
	{
		typedef InventoryEntryData* _Create_t(TESForm* a_item, UInt32 a_count);
		static _Create_t* _Create = reinterpret_cast<_Create_t*>(GetFnAddr(&::InventoryEntryData::Create));
		return _Create(a_item, a_count);
	}


	void InventoryEntryData::Delete(void)
	{
		typedef void _Delete_t(InventoryEntryData* a_this);
		static _Delete_t* _Delete = reinterpret_cast<_Delete_t*>(GetFnAddr(&::InventoryEntryData::Delete));
		_Delete(this);
	}


	void InventoryEntryData::GetExtraWornBaseLists(BaseExtraList*& a_pWornBaseListOut, BaseExtraList*& a_pWornLeftBaseListOut)
	{
		typedef void _GetExtraWornBaseLists_t(InventoryEntryData* a_this, BaseExtraList*& a_pWornBaseListOut, BaseExtraList*& a_pWornLeftBaseListOut);
		static _GetExtraWornBaseLists_t* _GetExtraWornBaseLists = reinterpret_cast<_GetExtraWornBaseLists_t*>(GetFnAddr(&::InventoryEntryData::GetExtraWornBaseLists));
		_GetExtraWornBaseLists(this, a_pWornBaseListOut, a_pWornLeftBaseListOut);
	}


	void InventoryEntryData::GetEquipItemData(::InventoryEntryData::EquipData& a_stateOut, SInt32 a_itemId, SInt32 a_baseCount)
	{
		typedef void _GetEquipItemData_t(InventoryEntryData* a_this, ::InventoryEntryData::EquipData& a_stateOut, SInt32 a_itemId, SInt32 a_baseCount);
		static _GetEquipItemData_t* _GetEquipItemData = reinterpret_cast<_GetEquipItemData_t*>(GetFnAddr(&::InventoryEntryData::GetEquipItemData));
		_GetEquipItemData(this, a_stateOut, a_itemId, a_baseCount);
	}


	const char* InventoryEntryData::GenerateName()
	{
		typedef const char* _GenerateName_t(InventoryEntryData* a_this);
		static uintptr_t* ptr = reinterpret_cast<uintptr_t*>(reinterpret_cast<::InventoryEntryData*>(this)->_GenerateName_GetPtr());
		static _GenerateName_t* _GenerateName = reinterpret_cast<_GenerateName_t*>(*ptr);
		return _GenerateName(this);
	}


	SInt32 InventoryEntryData::GetValue()
	{
		typedef SInt32 _GetValue_t(InventoryEntryData* a_this);
		static uintptr_t* ptr = reinterpret_cast<uintptr_t*>(reinterpret_cast<::InventoryEntryData*>(this)->_GetValue_GetPtr());
		static _GetValue_t* _GetValue = reinterpret_cast<_GetValue_t*>(*ptr);
		return _GetValue(this);
	}


	bool InventoryEntryData::IsOwnedBy(TESForm* a_actor, bool a_unk1)
	{
		typedef bool _IsOwnedBy_t(InventoryEntryData* a_this, TESForm* a_actor, bool a_unk1);
		static uintptr_t* ptr = reinterpret_cast<uintptr_t*>(reinterpret_cast<::InventoryEntryData*>(this)->_IsOwnedBy_GetPtr());
		static _IsOwnedBy_t* _IsOwnedBy = reinterpret_cast<_IsOwnedBy_t*>(*ptr);
		return _IsOwnedBy(this, a_actor, a_unk1);
	}


	UInt32 InventoryEntryData::GetSoulLevel()
	{
		typedef UInt32 _GetSoulLevel_t(InventoryEntryData* a_this);
		static uintptr_t* ptr = reinterpret_cast<uintptr_t*>(reinterpret_cast<::InventoryEntryData*>(this)->_GetSoulLevel_GetPtr());
		static _GetSoulLevel_t* _GetSoulLevel = reinterpret_cast<_GetSoulLevel_t*>(*ptr);
		return _GetSoulLevel(this);
	}


	bool InventoryEntryData::IsOwnedBy(TESForm* a_actor, TESForm* a_itemOwner, bool a_unk1)
	{
		return _IsOwnedBy(this, a_actor, a_itemOwner, a_unk1);
	}


	TESForm* InventoryEntryData::GetOwner()
	{
		return _GetOwner(this);
	}


	float InventoryEntryData::GetWeight()
	{
		return _GetWeight(this);
	}


	RelocAddr<InventoryEntryData::_IsOwnedBy_t*> InventoryEntryData::_IsOwnedBy(INVENTORY_ENTRY_DATA_IS_OWNED_BY);
	RelocAddr<InventoryEntryData::_GetOwner_t*> InventoryEntryData::_GetOwner(INVENTORY_ENTRY_DATA_GET_OWNER);
	RelocAddr<InventoryEntryData::_GetWeight_t*> InventoryEntryData::_GetWeight(INVENTORY_ENTRY_DATA_GET_WEIGHT);
}
