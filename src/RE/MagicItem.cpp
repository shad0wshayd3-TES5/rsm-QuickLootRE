#include "RE/MagicItem.h"

#include "skse64/GameObjects.h"  // MagicItem


namespace RE
{
	MagicItem::EffectItem* MagicItem::GetCostliestEffectItem(int a_arg1, bool a_arg2)
	{
		return reinterpret_cast<EffectItem*>(CALL_MEMBER_FN(reinterpret_cast<::MagicItem*>(this), GetCostliestEffectItem)(a_arg1, a_arg2));
	}


	float MagicItem::GetEffectiveMagickaCost(Character* a_caster)
	{
		return CALL_MEMBER_FN(reinterpret_cast<::MagicItem*>(this), GetEffectiveMagickaCost)(reinterpret_cast<::Character*>(a_caster));
	}
}
