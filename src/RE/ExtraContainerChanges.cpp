#include "RE/ExtraContainerChanges.h"

#include "skse64/GameExtraData.h"  // ExtraContainerChanges, ExtraContainerChanges::Data


namespace RE
{
	ExtraContainerChanges::Data::Data(TESObjectREFR* ref)
	{
		CALL_MEMBER_FN(this, ctor)(ref);
	}


	InventoryEntryData* ExtraContainerChanges::Data::FindItemEntry(TESForm* item)
	{
		return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->FindItemEntry(item));
	}


	InventoryEntryData* ExtraContainerChanges::Data::CreateEquipEntryData(TESForm* item)
	{
		return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->CreateEquipEntryData(item));
	}


	void ExtraContainerChanges::Data::GetEquipItemData(::InventoryEntryData::EquipData& stateOut, TESForm* item, SInt32 itemId)
	{
		reinterpret_cast<::ExtraContainerChanges::Data*>(this)->GetEquipItemData(stateOut, item, itemId);
	}


	void ExtraContainerChanges::Data::SetUniqueID(::BaseExtraList* itemList, TESForm* oldForm, TESForm* newForm)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), SetUniqueID)(itemList, oldForm, newForm);
	}


	void ExtraContainerChanges::Data::TransferItemUID(::BaseExtraList* extraList, TESForm* oldForm, TESForm* newForm, UInt32 unk1)
	{
		CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), TransferItemUID)(extraList, oldForm, newForm, unk1);
	}


	void ExtraContainerChanges::Data::InitContainer()
	{
		CALL_MEMBER_FN(this, InitContainer)();
	}


	::ExtraContainerChanges::FoundEquipData ExtraContainerChanges::FindEquipped(FormMatcher& matcher, bool isWorn, bool isWornLeft)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindEquipped(matcher, isWorn, isWornLeft);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(SInt32 hotkey)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(hotkey);
	}


	::ExtraContainerChanges::FoundHotkeyData ExtraContainerChanges::FindHotkey(TESForm* form)
	{
		return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(form);
	}
}
