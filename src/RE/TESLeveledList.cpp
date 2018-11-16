#include "RE/TESLeveledList.h"

#include "skse64/GameTypes.h"  // tArray

#include "Offsets.h"

#include "RE/BSTArray.h"  // BSScrapArray


namespace RE
{
	bool TESLeveledList::Calculate(UInt16 a_level, UInt16 a_count, BSScrapArray<CalculatedResult>& a_result, bool a_ignoreChanceValue, bool a_unk5)
	{
		return _Calculate_Impl(this, a_level, a_count, a_result, a_ignoreChanceValue, a_unk5);
	}


	RelocAddr<TESLeveledList::_Calculate_Impl_t*> TESLeveledList::_Calculate_Impl(TES_LEVELED_LIST_CALCULATE_IMPL);
}
