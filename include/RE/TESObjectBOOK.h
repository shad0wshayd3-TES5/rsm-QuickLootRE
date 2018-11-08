#pragma once

#include "skse64/GameFormComponents.h"  // TESFullName, TESModelTextureSwap, TESIcon, TESValueForm, TESWeightForm, TESDescription, BGSDestructibleObjectForm, BGSMessageIcon, BGSPickupPutdownSounds, BGSKeywordForm
#include "skse64/GameObjects.h"  // TESObjectBOOK

#include "RE/TESBoundObject.h"  // TESBoundObject

class SpellItem;
class TESObjectSTAT;


namespace RE
{
	class TESObjectBOOK : public TESBoundObject
	{
	public:
		enum { kTypeID = kFormType_Book };

		// parents
		TESFullName					fullName;		// 30
		TESModelTextureSwap			texSwap;		// 40
		TESIcon						icon;			// 78
		TESValueForm				value;			// 88
		TESWeightForm				weight;			// 98
		TESDescription				description;	// A8
		BGSDestructibleObjectForm	destructible;	// B8
		BGSMessageIcon				messageIcon;	// C8
		BGSPickupPutdownSounds		pickupSounds;	// E0
		BGSKeywordForm				keyword;		// F8


		inline bool IsRead() const	{ return (data.flags & Data::kType_Read) != 0; }


		struct Data
		{
			enum
			{
				kType_None			= 0,
				kType_Skill			= 1 << 0,
				kType_CantBeTaken	= 1 << 1,
				kType_Spell			= 1 << 2,	// takes priority over skill
				kType_Read			= 1 << 3,	// set once the book is equipped by the player, along with the CHANGE_BOOK_READ (0x40) change flag
			};

			UInt8	flags;		// 0
			UInt8	type;		// 1
			UInt16	unk02;		// 2, probably padding too
			UInt32	unk04;		// 4, probably padding (SE)

			union
			{
				UInt32		skill;
				SpellItem*	spell;
			} teaches;			// 8

			// make some sense of the flags field so we know what's in the union
			UInt32	GetSanitizedType(void)	{ return reinterpret_cast<::TESObjectBOOK*>(this)->data.GetSanitizedType(); }
		};


		// members
		Data			data;			// 110
		TESObjectSTAT*	bookStat;		// 120
		TESDescription	description2;	// 128
	};

	STATIC_ASSERT(sizeof(TESObjectBOOK) == 0x138);
}
