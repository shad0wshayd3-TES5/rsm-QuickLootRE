#pragma once

#include "skse64/GameFormComponents.h"  // TESContainer, TESSpellList, TESAIForm, TESFullName, BGSDestructibleObjectForm, BGSSkinForm, BGSKeywordForm, BGSAttackDataForm, BGSPerkRankArray

#include "RE/TESActorBaseData.h"  // TESActorBaseData
#include "RE/ActorValueOwner.h"  // ActorValueOwner
#include "RE/TESBoundAnimObject.h"  // TESBoundAnimObject

class TESCombatStyle;


namespace RE
{
	class TESActorBase : public TESBoundAnimObject
	{
	public:
		// parents
		TESActorBaseData			actorData;			// 030
		TESContainer				container;			// 088
		TESSpellList				spellList;			// 0A0
		TESAIForm					aiForm;				// 0B0
		TESFullName					fullName;			// 0D8
		ActorValueOwner				actorValueOwner;	// 0E8
		BGSDestructibleObjectForm	destructible;		// 0F0
		BGSSkinForm					skinForm;			// 100
		BGSKeywordForm				keyword;			// 110
		BGSAttackDataForm			attackData;			// 128
		BGSPerkRankArray			perkRanks;			// 138


		virtual bool				Unk_52(void);
		virtual TESCombatStyle*		GetCombatStyle();
		virtual void				SetCombatStyle(TESCombatStyle* a_arg);
		virtual TESActorBase*		Unk_55(void);
	};
	STATIC_ASSERT(offsetof(TESActorBase, keyword) == 0x110);
	STATIC_ASSERT(sizeof(TESActorBase) == 0x150);
}
