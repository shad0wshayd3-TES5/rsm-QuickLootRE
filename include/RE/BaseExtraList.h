#pragma once

#include "skse64/GameBSExtraData.h"  // BaseExtraList

#include "Offsets.h"

#include "RE/ExtraContainerChanges.h"  // ExtraContainerChanges::Data

class BSExtraData;
class TESForm;


namespace RE
{
	class BaseExtraList
	{
	public:
		BaseExtraList();
		~BaseExtraList();

		bool			HasType(UInt32 type);
		void			MarkType(UInt32 type, bool bCleared);
		bool			Remove(UInt8 type, BSExtraData* toRemove);
		bool			Add(UInt8 type, BSExtraData* toAdd);
		bool			CheckContainerExtraData(bool isEquipped);
		const char*		GetDisplayName(TESForm* type);
		BSExtraData*	GetByType(UInt32 type);

		void			SetInventoryChanges(ExtraContainerChanges::Data* changes);
		UInt32			GetAshPileRefHandle(UInt32& refHandle);


		BSExtraData*						m_data;		// 00
		::BaseExtraList::PresenceBitfield*	m_presence;	// 08
		BSReadWriteLock						m_lock;

	private:
		MEMBER_FN_PREFIX(BaseExtraList);
		DEFINE_MEMBER_FN(SetInventoryChanges_Impl, void, BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL, ExtraContainerChanges::Data* changes);
		DEFINE_MEMBER_FN(GetAshPileRefHandle_Impl, UInt32, BASE_EXTRA_LIST_GET_ASH_PILE_REF_HANDLE_IMPL, UInt32& refHandle);
	};
	STATIC_ASSERT(sizeof(BaseExtraList) == 0x18);
}
