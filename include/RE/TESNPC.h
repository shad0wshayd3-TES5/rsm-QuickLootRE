#pragma once

#include "skse64/GameFormComponents.h"  // TESRaceForm, BGSOverridePackCollection
#include "skse64/GameTypes.h"  // BSFixedString

#include "RE/TESActorBase.h"  // TESActorBase

class BGSColorForm;
class BGSHeadPart;
class BGSOutfit;
class BGSTextureSet;
class BSFaceGenNiNode;
class TESClass;
class TESCombatStyle;
class TESFaction;
class TESObjectARMO;


namespace RE
{
	class TESNPC : public TESActorBase
	{
	public:
		// parents
		TESRaceForm					race;				// 150
		BGSOverridePackCollection	overridePacks;		// 160
		void*						menuOpenCloseEvent;	// 188 - MenuOpenCloseEvent


		enum { kTypeID = kFormType_NPC };


		struct FaceMorphs
		{
			enum
			{
				kNumOptions = 19,
				kNumPresets = 4,
			};


			float	option[kNumOptions];
			UInt32	presets[kNumPresets];
		};


		struct HeadData
		{
			BGSColorForm*	hairColor;
			BGSTextureSet*	headTexture;	// Only seems to apply to the player
		};


		struct Color
		{
			// 797979 Transparent
			UInt8	red;	// 0
			UInt8	green;	// 1
			UInt8	blue;	// 2
		};


		struct MorphAction
		{
			BSFaceGenNiNode*	faceNode;
			TESNPC*				npc;
			const char**		morphName;
			float				value;
		};


		inline char			GetSex()																					{ return CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), GetSex)(); }
		inline bool			HasOverlays()																				{ return CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), HasOverlays)(); }
		inline void			ChangeHeadPart(BGSHeadPart* a_target)														{ CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), ChangeHeadPart)(a_target); }
		inline void			ApplyMorph(MorphAction* a_morphAction)														{ CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), ApplyMorph)(reinterpret_cast<::TESNPC::MorphAction*>(a_morphAction)); }
		inline void			UpdateNeck(BSFaceGenNiNode* a_faceNode)														{ CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), UpdateNeck)(a_faceNode); }
		inline void			SetSkinFromTint(NiColorA* a_result, TintMask* a_tintMask, UInt32 a_compute, UInt32 a_unk1)	{ CALL_MEMBER_FN(reinterpret_cast<::TESNPC*>(this), SetSkinFromTint)(a_result, a_tintMask, a_compute, a_unk1); }
		inline void			SetFaceTexture(BGSTextureSet* a_textureSet)													{ reinterpret_cast<::TESNPC*>(this)->SetFaceTexture(a_textureSet); }
		inline void			SetHairColor(BGSColorForm* a_hairColor)														{ reinterpret_cast<::TESNPC*>(this)->SetHairColor(a_hairColor); }
		inline BGSHeadPart*	GetHeadPartByType(UInt32 a_type)															{ return reinterpret_cast<::TESNPC*>(this)->GetCurrentHeadPartByType(a_type); }
		inline BGSHeadPart*	GetHeadPartOverlayByType(UInt32 a_type)														{ return reinterpret_cast<::TESNPC*>(this)->GetHeadPartOverlayByType(a_type); }
		inline BGSHeadPart*	GetCurrentHeadPartByType(UInt32 a_type)														{ return reinterpret_cast<::TESNPC*>(this)->GetCurrentHeadPartByType(a_type); }
		inline TESNPC*		GetRootTemplate()																			{ return reinterpret_cast<TESNPC*>(reinterpret_cast<::TESNPC*>(this)->GetRootTemplate()); }

		inline TESRace*		GetRace()																					{ return race.race; }


		// members
		UInt8			unk190[0x12];			// 190 - init'd to 5
		UInt8			unk1A2[0x12];			// 1A2 - init'd to 0
		UInt8			pad1B4[0x1BA - 0x1B4];	// 1B4
		UInt16			unk1BA;					// 1BA
		UInt32			pad1BC;					// 1BC
		TESClass*		npcClass;				// 1C0
		HeadData*		headData;				// 1C8
		void*			unk1D0;					// 1D0
		TESCombatStyle*	combatStyle;			// 1D8
		UInt32			unk1E0;					// 1E0
		UInt32			pad1E4;					// 1E4
		TESRace*		overlayRace;			// 1E8
		TESNPC*			nextTemplate;			// 1F0
		float			height;					// 1F8 - init'd to 1
		float			weight;					// 1FC - init'd to 50
		void*			unk200;					// 200
		BSFixedString	shortName;				// 208
		TESObjectARMO*	skinFar;				// 210
		BGSOutfit*		defaultOutfit;			// 218
		BGSOutfit*		sleepOutfit;			// 220
		void*			unk228;					// 228
		TESFaction*		faction;				// 230
		BGSHeadPart**	headparts;				// 238
		UInt8			numHeadParts;			// 240
		UInt8			unk241;					// 241 - init'd to 1
		UInt8			unk242;					// 242 - New in SE?
		UInt8			unk243;					// 243
		UInt8			unk244;					// 244 - New in SE?
		UInt8			unk245;					// 245 - init'd to 1
		Color			color;					// 246
		UInt8			pad249[7];				// 249
		void*			unk250;					// 250
		FaceMorphs*		faceMorph;				// 258
		void*			unk260;					// 260
	};
	STATIC_ASSERT(offsetof(TESNPC, nextTemplate) == 0x1F0);
	STATIC_ASSERT(sizeof(TESNPC) == 0x268);
}
