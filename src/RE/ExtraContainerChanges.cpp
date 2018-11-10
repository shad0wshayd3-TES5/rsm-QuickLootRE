#include "RE/ExtraContainerChanges.h"

#include "skse64/GameExtraData.h"  // ExtraContainerChanges, ExtraContainerChanges::Data


namespace RE
{
	ExtraContainerChanges::Data::Data(TESObjectREFR* a_ref)
	{
		_ctor(this, a_ref);
	}


	InventoryEntryData* ExtraContainerChanges::Data::FindItemEntry(TESForm* a_item)
	{
		return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->FindItemEntry(a_item));
	}


	InventoryEntryData* ExtraContainerChanges::Data::CreateEquipEntryData(TESForm* a_item)
	{
		return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->CreateEquipEntryData(a_item));
	}


	void ExtraContainerChanges::Data::GetEquipItemData(::InventoryEntryData::EquipData& a_stateOut, TESForm* a_item, SInt32 a_itemId)
	{
		reinterpret_cast<::ExtraContainerChanges::Data*>(this)->GetEquipItemData(a_stateOut, a_item, a_itemId);
	}


	void ExtraContainerChanges::Data::SetUniqueID(::BaseExtraList* a_itemList, TESForm* a_oldForm, TESForm* a_newForm)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), SetUniqueID)(a_itemList, a_oldForm, a_newForm);
	}


	void ExtraContainerChanges::Data::TransferItemUID(::BaseExtraList* a_extraList, TESForm* a_oldForm, TESForm* a_newForm, UInt32 a_unk1)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), TransferItemUID)(a_extraList, a_oldForm, a_newForm, a_unk1);
	}


	void ExtraContainerChanges::Data::InitContainer()
	{
		_InitContainer(this);
	}


	RelocAddr<ExtraContainerChanges::Data::_ctor_t*> ExtraContainerChanges::Data::_ctor(EXTRA_CONTAINER_CHANGES_DATA_CTOR);
	RelocAddr<ExtraContainerChanges::Data::_InitContainer_t*> ExtraContainerChanges::Data::_InitContainer(EXTRA_CONTAINER_CHANGES_DATA_INIT_CONTAINER);


	::ExtraContainerChanges::FoundEquipData ExtraContainerChanges::FindEquipped(FormMatcher& a_matcher, bool a_isWorn, bool a_isWornLeft)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindEquipped(a_matcher, a_isWorn, a_isWornLeft);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(SInt32 a_hotkey)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(a_hotkey);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(TESForm* a_form)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(a_form);
	}
}
