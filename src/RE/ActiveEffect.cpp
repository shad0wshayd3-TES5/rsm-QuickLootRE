#include "RE/ActiveEffect.h"


namespace RE
{
	EffectSetting* ActiveEffect::GetBaseObject() const
	{
		return effect->mgef;
	}
}
