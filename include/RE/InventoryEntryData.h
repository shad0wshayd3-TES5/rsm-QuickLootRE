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

		inline static InventoryEntryData*	Create(TESForm* item, UInt32 count)																{ return reinterpret_cast<InventoryEntryData*>(::InventoryEntryData::Create(item, count)); }
		inline void							Delete(void)																					{ reinterpret_cast<::InventoryEntryData*>(this)->Delete(); }
		inline void							GetExtraWornBaseLists(BaseExtraList** pWornBaseListOut, BaseExtraList** pWornLeftBaseListOut)	{ return reinterpret_cast<::InventoryEntryData*>(this)->GetExtraWornBaseLists(reinterpret_cast<::BaseExtraList**>(pWornBaseListOut), reinterpret_cast<::BaseExtraList**>(pWornLeftBaseListOut)); }
		inline void							GetEquipItemData(::InventoryEntryData::EquipData& stateOut, SInt32 itemId, SInt32 baseCount)	{ reinterpret_cast<::InventoryEntryData*>(this)->GetEquipItemData(stateOut, itemId, baseCount); }
		inline const char*					GenerateName()																					{ return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GenerateName)(); }
		inline SInt32						GetValue()																						{ return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GetValue)(); }
		inline bool							IsOwnedBy(TESForm* actor, bool unk1)															{ return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), IsOwnedBy)(actor, unk1); }
		inline UInt32						GetSoulLevel()																					{ return CALL_MEMBER_FN(reinterpret_cast<::InventoryEntryData*>(this), GetSoulLevel)(); }

		inline bool							IsOwnedBy(TESForm* actor, TESForm* itemOwner, bool unk1)										{ return CALL_MEMBER_FN(this, IsOwnedBy)(actor, itemOwner, unk1); }
		inline TESForm*						GetOwner()																						{ return CALL_MEMBER_FN(this, GetOwner)(); }


		// members
		TESForm*		type;			// 00
		ExtendDataList*	extendDataList;	// 08
		SInt32			countDelta;		// 10

	private:
		MEMBER_FN_PREFIX(InventoryEntryData);
		DEFINE_MEMBER_FN(IsOwnedBy, bool, INVENTORY_ENTRY_DATA_IS_OWNED_BY, TESForm* actor, TESForm* itemOwner, bool unk1);
		DEFINE_MEMBER_FN(GetOwner, TESForm*, INVENTORY_ENTRY_DATA_GET_OWNER);
	};
}
