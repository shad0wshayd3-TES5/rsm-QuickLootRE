#pragma once

#include "skse64/GameBSExtraData.h"  // ExtendDataList
#include "skse64/GameExtraData.h"  // InventoryEntryData

#include "Offsets.h"

class TESForm;


namespace RE
{
	class BaseExtraList;


	class InventoryEntryData
	{
	public:
		InventoryEntryData(TESForm * item, UInt32 count) : type(item), countDelta(count), extendDataList(0) {}

		static InventoryEntryData*	Create(TESForm* item, UInt32 count);
		void						Delete(void);
		void						GetExtraWornBaseLists(BaseExtraList** pWornBaseListOut, BaseExtraList** pWornLeftBaseListOut);
		void						GetEquipItemData(::InventoryEntryData::EquipData& stateOut, SInt32 itemId, SInt32 baseCount);
		const char*					GenerateName();
		SInt32						GetValue();
		bool						IsOwnedBy(TESForm* actor, bool unk1);
		UInt32						GetSoulLevel();

		bool						IsOwnedBy(TESForm* actor, TESForm* itemOwner, bool unk1);
		TESForm*					GetOwner();
		float						GetWeight();


		// members
		TESForm*		type;			// 00
		ExtendDataList*	extendDataList;	// 08
		SInt32			countDelta;		// 10

	private:
		MEMBER_FN_PREFIX(InventoryEntryData);
		DEFINE_MEMBER_FN(IsOwnedBy, bool, INVENTORY_ENTRY_DATA_IS_OWNED_BY, TESForm* actor, TESForm* itemOwner, bool unk1);
		DEFINE_MEMBER_FN(GetOwner, TESForm*, INVENTORY_ENTRY_DATA_GET_OWNER);
		DEFINE_MEMBER_FN(GetWeight, float, INVENTORY_ENTRY_DATA_GET_WEIGHT);	// { return TESWeightForm::weight; } - Only for certain formtypes
	};
}
