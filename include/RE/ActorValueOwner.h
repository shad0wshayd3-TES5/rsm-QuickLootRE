#pragma once

#include "skse64_common/Utilities.h"  // MEMBER_FN_PREFIX, DEFINE_MEMBER_FN

#include "Offsets.h"

#include "RE/EffectSetting.h"  // EffectSetting::Properties::ActorValue


namespace RE
{
	class ActorValueOwner
	{
	private:
		typedef EffectSetting::Properties::ActorValue ActorValue;

	public:
		virtual ~ActorValueOwner();

		virtual float	GetActorValueCurrent(ActorValue a_akValue);
		virtual float	GetActorValueMaximum(ActorValue a_akValue);
		virtual float	GetActorValueBase(ActorValue a_akValue);
		virtual void	SetActorValueBase(ActorValue a_akValue, float value);
		virtual void	ModActorValueBase(ActorValue a_akValue, float value);
		virtual void	Unk_06(UInt32 arg0, UInt32 arg1, UInt32 arg2);
		virtual void	SetActorValueCurrent(ActorValue a_akValue, float value);
		virtual bool	Unk_08(void);

		inline float	GetPlayerActorValueCurrent(ActorValue a_akValue)	{ return CALL_MEMBER_FN(this, GetPlayerActorValueCurrent)(a_akValue); }


	private:
		MEMBER_FN_PREFIX(ActorValueOwner);
		DEFINE_MEMBER_FN(GetPlayerActorValueCurrent, float, ACTOR_VALUE_OWNER_GET_PLAYER_ACTOR_VALUE_CURRENT, ActorValue a_akValue);
	};
	STATIC_ASSERT(sizeof(ActorValueOwner) == 0x8);
}
