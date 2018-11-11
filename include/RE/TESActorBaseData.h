#pragma once

#include "skse64/GameFormComponents.h"  // BaseFormComponent

class BGSVoiceType;


namespace RE
{
	class TESFaction;


	class TESActorBaseData : public BaseFormComponent
	{
	public:
		enum Flag : UInt32
		{
			kFlag_Female				= 1 << 0,
			kFlag_Essential				= 1 << 1,
			kFlag_Unk02					= 1 << 2,
			kFlag_Respawn				= 1 << 3,
			kFlag_AutoCalc				= 1 << 4,
			kFlag_Unique				= 1 << 5,
			kFlag_DontAffectStealth		= 1 << 6,
			kFlag_PCLevelMult			= 1 << 7,
			kFlag_Unk08					= 1 << 8,
			kFlag_Unk09					= 1 << 9,
			kFlag_Unk10					= 1 << 10,
			kFlag_Protect				= 1 << 11,
			kFlag_Unk12					= 1 << 12,
			kFlag_unk13					= 1 << 13,
			kFlag_Summonable			= 1 << 14,
			kFlag_Unk15					= 1 << 15,
			kFlag_DoesntBleed			= 1 << 16,
			kFlag_Unk17					= 1 << 17,
			kFlag_BleedoutOverride		= 1 << 18,
			kFlag_OppositeGenderanims	= 1 << 19,
			kFlag_SimpleActor			= 1 << 20,
			kFlag_Unk21					= 1 << 21,
			kFlag_Unk22					= 1 << 22,
			kFlag_Unk23					= 1 << 23,
			kFlag_Unk24					= 1 << 24,
			kFlag_Unk25					= 1 << 25,
			kFlag_Unk26					= 1 << 26,
			kFlag_Unk27					= 1 << 27,
			kFlag_Unk28					= 1 << 28,
			kFlag_Ghost					= 1 << 29,
			kFlag_Unk30					= 1 << 30,
			kFlag_Invulnerable			= (UInt32)(1 << 31)
		};


		struct FactionInfo
		{
			TESFaction*	faction;	// 0
			SInt8		rank;		// 8
			UInt8		pad9[7];	// 9
		};


		virtual void			Unk_04(UInt32 arg);
		virtual bool			GetFlag20000000(void);
		virtual bool			GetFlag80000000(void);
		virtual bool			Unk_07(void);
		virtual BGSVoiceType*	GetVoiceType();

		bool					IsFemale();
		bool					IsEssential();
		bool					Respawns();
		bool					UsesAutoCalc();
		bool					IsUnique();
		bool					AffectsStealth();
		bool					HasPCLevelMult();
		bool					Protects();
		bool					IsSummonable();
		bool					Bleeds();
		bool					HasBleedoutOverride();
		bool					UsesOppositeGenderAnims();
		bool					IsSimpleActor();
		bool					IsGhost();
		bool					IsInvulnerable();


		Flag				flags;		// 08 - init'd to 0
		UInt16				unk0C;		// 0C - init'd to 0
		UInt16				unk0E;		// 0E - init'd to 0
		UInt16				level;		// 10 - init'd to 1	(CK Multiplier * 1000 if PCLevelMult is true)
		UInt16				minLevel;	// 12 - init'd to 0
		UInt16				maxLevel;	// 14 - init'd to 0
		UInt16				unk16;		// 16 - init'd to 0x64 (100)
		UInt16				unk18;		// 18 - init'd to iBaseDisposition (35)
		UInt16				unk1A;		// 1A - init'd to 0
		UInt16				unk1C;		// 1C - init'd to 0
		UInt16				unk1E;		// 1E - init'd to 0
		void*				unk20;		// 20
		BGSVoiceType*		voiceType;	// 28
		UInt64				unk30;		// 30
		UInt32				unk38;		// 38 - init'd to 0
		UInt32				pad3C;		// 3C
		tArray<FactionInfo>	factions;	// 40
	};
	STATIC_ASSERT(sizeof(TESActorBaseData) == 0x58);
}
