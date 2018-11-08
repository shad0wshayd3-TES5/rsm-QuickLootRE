#pragma once

#include "skse64/GameFormComponents.h"  // TESFullName, TESModelTextureSwap, TESIcon, BGSMessageIcon, TESWeightForm, TESValueForm, BGSDestructibleObjectForm, BGSEquipType

#include "RE/TESBoundAnimObject.h"  // TESBoundAnimObject


namespace RE
{
	class TESObjectLIGH : public TESBoundAnimObject
	{
	public:
		// Parents
		TESFullName					fullName;		// 030
		TESModelTextureSwap			texSwap;		// 040
		TESIcon						icon;			// 078
		BGSMessageIcon				messageIcon;	// 088
		TESWeightForm				weight;			// 0A0
		TESValueForm				value;			// 0B0
		BGSDestructibleObjectForm	destructible;	// 0C0
		BGSEquipType				equipType;		// 0D0


		enum { kTypeID = kFormType_Light };


		enum Flag : UInt32
		{
			kFlag_Dynamic				= 1 << 0,
			kFlag_CanBeCarried			= 1 << 1,
			kFlag_Flicker				= 1 << 3,
			kFlag_Pulse					= 1 << 7,
			kFlag_ShadowSpotlight		= 1 << 10,
			kFlag_Hemisphere			= 1 << 11,
			kFlag_ShadowOmniDirectional	= 1 << 12,
			kFlag_PortalStrict			= 1 << 13
		};


		struct DataE0
		{
			struct Color
			{
				UInt8	red;	// 0
				UInt8	green;	// 1
				UInt8	blue;	// 2
				UInt8	pad3;	// 3
			};


			struct FlickerEffect
			{
				UInt32	period;				// 0
				UInt32	intensityAmplitude;	// 4
				UInt32	movementAmplitude;	// 8
			};


			SInt32			time;			// 00
			UInt32			radius;			// 04
			Color			color;			// 08
			Flag			flags;			// 0C
			UInt32			fallofExponent;	// 10
			float			FOV;			// 14 - 90
			float			nearClip;		// 18 - .001
			FlickerEffect	flickerEffect;	// 1C
		};


		struct Data118
		{
			UInt64	unk0;	// 00
			UInt32	unk4;	// 08
			UInt32	padC;	// 0C
			UInt64	unk8;	// 10
		};


		virtual ~TESObjectLIGH();

		inline bool CanBeCarried() const { return (unkE0.flags & kFlag_CanBeCarried) != 0; }


		DataE0	unkE0;		// 0E0 - DATA
		float	fadeValue;	// 108 - FNAM
		UInt32	pad10C;		// 10C - SNAM
		UInt64	unk110;		// 110
		Data118	unk118;		// 118
	};
}
