#include "RE/ActorValueOwner.h"


namespace RE
{
	float ActorValueOwner::GetPlayerActorValueCurrent(ActorValue a_akValue)
	{
		return CALL_MEMBER_FN(this, GetPlayerActorValueCurrent)(a_akValue);
	}
}
