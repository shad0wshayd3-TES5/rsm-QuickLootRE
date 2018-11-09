#pragma once

#include "skse64/GameTypes.h"  // tList

#include "Offsets.h"

#include "RE/ActiveEffect.h"  // ActiveEffect
#include "RE/EffectSetting.h"  // EffectSetting::Properties::Archetype


namespace RE
{
	class Character;


	class MagicTarget
	{
	public:
		virtual ~MagicTarget();

		virtual UInt8					Unk_01(int);
		virtual Character*				Unk_02(void);
		virtual UInt8					Unk_03(void);
		virtual bool					Unk_04(void);
		virtual int						Unk_05(int);
		virtual bool					Unk_06(void);
		virtual tList<ActiveEffect>*	GetActiveEffects();
		virtual int						Unk_08(int);
		virtual void					Unk_09(int);
		virtual double					Unk_0A(int, int, int);
		virtual UInt8					Unk_0B(int, int, int);

		bool							HasEffectWithArchetype(EffectSetting::Properties::Archetype a_type);


		void*	unk04;	// 08
		void*	unk08;	// 10
	};
	STATIC_ASSERT(sizeof(MagicTarget) == 0x18);
}
