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
			explicit Data(TESObjectREFR* ref);

			InventoryEntryData*	FindItemEntry(TESForm* item);
			InventoryEntryData*	CreateEquipEntryData(TESForm* item);
			void				GetEquipItemData(::InventoryEntryData::EquipData& stateOut, TESForm* item, SInt32 itemId);
			void				SetUniqueID(::BaseExtraList* itemList, TESForm* oldForm, TESForm* newForm);
			void				TransferItemUID(::BaseExtraList* extraList, TESForm* oldForm, TESForm* newForm, UInt32 unk1);

			void				InitContainer();


			EntryDataList*	objList;		// 00
			TESObjectREFR*	owner;			// 08
			float			totalWeight;	// 10
			float			armorWeight;	// 14
			bool			changed;		// 18
			UInt8			pad19[0x3];		// 19

		private:
			MEMBER_FN_PREFIX(Data);
			DEFINE_MEMBER_FN(ctor, ExtraContainerChanges::Data*, EXTRA_CONTAINER_CHANGES_DATA_CTOR, TESObjectREFR* ref);
			DEFINE_MEMBER_FN(InitContainer, void, EXTRA_CONTAINER_CHANGES_DATA_INIT_CONTAINER);
		};


		ExtraContainerChanges();
		virtual	~ExtraContainerChanges();

		::ExtraContainerChanges::FoundEquipData		FindEquipped(FormMatcher& matcher, bool isWorn = true, bool isWornLeft = true);
		::ExtraContainerChanges::FoundHotkeyData	FindHotkey(SInt32 hotkey);
		::ExtraContainerChanges::FoundHotkeyData	FindHotkey(TESForm* form);


		Data*	data;	// 10
	};
}
