#pragma once

#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/ScaleformCallbacks.h"  // FxDelegateHandler

class CallbackProcessor;
class GRefCountBase;
class UIMessage;


namespace RE
{
	class GFxMovieView;


	class IMenu : public FxDelegateHandler
	{
	public:
		enum Flag : UInt32
		{
			kFlag_PauseGame				= 1 << 0,
			kFlag_DoNotDeleteOnClose	= 1 << 1,
			kFlag_ShowCursor			= 1 << 2,
			kFlag_Unk0008				= 1 << 3,
			kFlag_Modal					= 1 << 4,
			kFlag_StopDrawingWorld		= 1 << 5,
			kFlag_Open					= 1 << 6,
			kFlag_PreventGameLoad		= 1 << 7,
			kFlag_Unk0100				= 1 << 8,
			kFlag_HideOther				= 1 << 9,
			kFlag_Unk0400				= 1 << 10,
			kFlag_DoNotPreventGameSave	= 1 << 11,
			kFlag_Unk1000				= 1 << 12,
			kFlag_ItemMenu				= 1 << 13,
			kFlag_StopCrosshairUpdate	= 1 << 14,
			kFlag_Unk8000				= 1 << 15,
			kFlag_Unk10000				= 1 << 16  // ignore mouse cursor?
		};


		struct BSUIScaleformData
		{
			virtual ~BSUIScaleformData() {}

			UInt32				unk08;	// 08
			void*				unk10;	// 10
		};


		struct UnkData1
		{
			BSFixedString		name;	// 00
			UInt32				unk04;	// 04
			BSUIScaleformData*	data;	// 08 - BSUIScaleformData
		};


		IMenu()	: view(0), menuDepth(3), flags(0), context(0x12), unk18(0)	{}
		virtual ~IMenu()													{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), dtor)(); }

		inline virtual void		Accept(CallbackProcessor* processor)		{}
		inline virtual void		Unk_02(void)								{}
		inline virtual void		Unk_03(void)								{}
		inline virtual UInt32	ProcessMessage(UIMessage* message)			{ return CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), ProcessMessage_internal)(message); }
		inline virtual void		NextFrame(UInt32 arg0, UInt32 arg1)			{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), NextFrame_internal)(arg0, arg1); }
		inline virtual void		Render(void)								{ reinterpret_cast<::IMenu*>(this)->Render(); }
		inline virtual void		Unk_07(void)								{}
		inline virtual void		InitMovie(void)								{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), InitMovie_internal)(reinterpret_cast<::GFxMovieView*>(view)); }


		GFxMovieView*	view;		// 10 - init'd to 0, a class, virtual fn 0x228 called in dtor
		UInt8			menuDepth;	// 18 - init'd to 3
		UInt8			pad19[3];	// 19
		UInt32			flags;		// 1C - init'd to 0
		UInt32			context;	// 20 - input context - init'd to 0x12 (kContext_Invalid)
		UInt32			pad24;		// 24
		GRefCountBase*	unk18;		// 28 - holds a reference
	};
	STATIC_ASSERT(offsetof(IMenu, view) == 0x10);
}
