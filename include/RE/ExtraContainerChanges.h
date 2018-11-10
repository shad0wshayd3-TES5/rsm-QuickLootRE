#pragma once

#include "skse64/GameExtraData.h"  // BSExtraData, ExtraContainerChanges, InventoryEntryData, InventoryEntryData::EquipData

#include "Offsets.h"

class BaseExtraList;
class TESForm;


namespace RE
{
	class EntryDataList;
	class InventoryEntryData;
	class TESObjectREFR;


	class ExtraContainerChanges : public BSExtraData
	{
	public:
		class Data
		{
		public:
			explicit Data(TESObjectREFR* a_ref);

			InventoryEntryData*	FindItemEntry(TESForm* a_item);
			InventoryEntryData*	CreateEquipEntryData(TESForm* a_item);
			void				GetEquipItemData(::InventoryEntryData::EquipData& a_stateOut, TESForm* a_item, SInt32 a_itemId);
			void				SetUniqueID(::BaseExtraList* a_itemList, TESForm* a_oldForm, TESForm* a_newForm);
			void				TransferItemUID(::BaseExtraList* a_extraList, TESForm* a_oldForm, TESForm* a_newForm, UInt32 a_unk1);

			void				InitContainer();


			EntryDataList*	objList;		// 00
			TESObjectREFR*	owner;			// 08
			float			totalWeight;	// 10
			float			armorWeight;	// 14
			bool			changed;		// 18
			UInt8			pad19[0x3];		// 19

		private:
			typedef ExtraContainerChanges::Data* _ctor_t(Data* a_this, TESObjectREFR* a_ref);
			static RelocAddr<_ctor_t*> _ctor;

			typedef void _InitContainer_t(Data* a_this);
			static RelocAddr<_InitContainer_t*> _InitContainer;
		};


		ExtraContainerChanges();
		virtual	~ExtraContainerChanges();

		::ExtraContainerChanges::FoundEquipData		FindEquipped(FormMatcher& a_matcher, bool a_isWorn = true, bool a_isWornLeft = true);
		::ExtraContainerChanges::FoundHotkeyData	FindHotkey(SInt32 a_hotkey);
		::ExtraContainerChanges::FoundHotkeyData	FindHotkey(TESForm* a_form);


		Data*	data;	// 10
	};
}
