#pragma once

#include "skse64/GameFormComponents.h"  // TESContainer, TESFullName, TESModelTextureSwap, TESWeightForm, BGSDestructibleObjectForm, BGSOpenCloseForm

#include "RE/TESBoundAnimObject.h"  // TESBoundAnimObject


namespace RE
{
	class TESObjectCONT : public TESBoundAnimObject
	{
	public:
		enum { kTypeID = kFormType_Container };

		virtual bool GetCrosshairText(TESObjectREFR* a_ref, BSString* a_dst, bool a_unk) override;


		// parents
		TESContainer				container;		// 30
		TESFullName					fullName;		// 48
		TESModelTextureSwap			texSwap;		// 58
		TESWeightForm				weight;			// 90
		BGSDestructibleObjectForm	destructible;	// A0
		BGSOpenCloseForm			openClose;		// B0

		// members
		UInt8	unkB8;		// B8
		UInt8	unkB9;		// B9
		UInt8	padBA[6];	// BA
		UInt64	padC0;		// C0
		UInt64	padC8;		// C8
	};
}
