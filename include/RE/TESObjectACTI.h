#pragma once

#include "skse64/GameFormComponents.h"  // TESFullName, TESModelTextureSwap, BGSDestructibleObjectForm, BGSOpenCloseForm, BGSKeywordForm

#include "RE/TESBoundAnimObject.h"  // TESBoundAnimObject

class BGSSoundDescriptorForm;
class BSString;
class TESWaterForm;


namespace RE
{
	class TESObjectREFR;


	class TESObjectACTI : public TESBoundAnimObject
	{
	public:
		enum { kTypeID = kFormType_Activator };


		virtual bool	GetCrosshairText(TESObjectREFR* a_ref, BSString* a_dst, bool a_unk) override;


		// parents
		TESFullName					fullName;		// 20-30
		TESModelTextureSwap			texSwap;		// 28-40
		BGSDestructibleObjectForm	destructible;	// 44-78
		BGSOpenCloseForm			openClose;		// 4C-88
		BGSKeywordForm				keyword;		// 50-90

		// members
		BGSSoundDescriptorForm*	loopingSound;		// A8
		BGSSoundDescriptorForm*	activationSound;	// B0
		TESWaterForm*			waterType;			// B8
		UInt16					flags;				// C0
		UInt8					padC2[6];			// C2
	};
}
