#pragma once

#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/ScaleformCallbacks.h"  // FxDelegateHandler

#include "RE/GFxMovieView.h"  // GFxMovieView
#include "RE/InputManager.h"  // InputManager::Context

class CallbackProcessor;
class GRefCountBase;
class UIMessage;


namespace RE
{
	class IMenu : public FxDelegateHandler
	{
	public:
		enum Flag : UInt32
		{
			kFlag_None					= 0,
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


		enum Result
		{
			kResult_Processed = 0,
			kResult_Abort,
			kResult_NotProcessed
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


		IMenu()	: view(0), menuDepth(3), flags(kFlag_None), context(InputManager::kContext_Invalid), unk18(0)	{}
		virtual ~IMenu()																						{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), dtor)(); }

		virtual void			Accept(CallbackProcessor* processor) override									{}
		virtual void			OnOpen()																		{}
		virtual void			Unk_03(void)																	{}
		virtual UInt32			ProcessMessage(UIMessage* message)												{ return CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), ProcessMessage_internal)(message); }
		virtual void			NextFrame(UInt32 arg0, UInt32 arg1)												{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), NextFrame_internal)(arg0, arg1); }
		virtual void			Render()																		{ if (view) { view->Display(); } }
		virtual void			Unk_07(void)																	{}
		virtual void			InitMovie()																		{ CALL_MEMBER_FN(reinterpret_cast<::IMenu*>(this), InitMovie_internal)(reinterpret_cast<::GFxMovieView*>(view)); }

		inline bool				PausesGame()																	{ return (flags & kFlag_PauseGame)				!= 0; }
		inline bool				DeletesOnClose()																{ return (flags & kFlag_DoNotDeleteOnClose)		== 0; }
		inline bool				ShowsCursor()																	{ return (flags & kFlag_ShowCursor)				!= 0; }
		inline bool				HasFlag0008()																	{ return (flags & kFlag_Unk0008)				!= 0; }
		inline bool				IsModal()																		{ return (flags & kFlag_Modal)					!= 0; }
		inline bool				StopsDrawingWorld()																{ return (flags & kFlag_StopDrawingWorld)		!= 0; }
		inline bool				IsOpen()																		{ return (flags & kFlag_Open)					!= 0; }
		inline bool				PreventsGameLoad()																{ return (flags & kFlag_PreventGameLoad)		!= 0; }
		inline bool				HasFlag0100()																	{ return (flags & kFlag_Unk0100)				!= 0; }
		inline bool				HidesOtherMenus()																{ return (flags & kFlag_HideOther)				!= 0; }
		inline bool				HasFlag0400()																	{ return (flags & kFlag_Unk0400)				!= 0; }
		inline bool				PreventsGameSave()																{ return (flags & kFlag_DoNotPreventGameSave)	== 0; }
		inline bool				HasFlag1000()																	{ return (flags & kFlag_Unk1000)				!= 0; }
		inline bool				IsItemMenu()																	{ return (flags & kFlag_ItemMenu)				!= 0; }
		inline bool				StopsCrosshairUpdates()															{ return (flags & kFlag_StopCrosshairUpdate)	!= 0; }
		inline bool				HasFlag8000()																	{ return (flags & kFlag_Unk8000)				!= 0; }
		inline bool				HasFlag10000()																	{ return (flags & kFlag_Unk10000)				!= 0; }


		GFxMovieView*			view;		// 10 - init'd to 0, view->Release() called in dtor
		UInt8					menuDepth;	// 18 - init'd to 3
		UInt8					pad19[3];	// 19
		Flag					flags;		// 1C - init'd to 0
		InputManager::Context	context;	// 20 - input context - init'd to 0x12 (kContext_Invalid)
		UInt32					pad24;		// 24
		GRefCountBase*			unk18;		// 28 - FxDelegate
	};
	STATIC_ASSERT(offsetof(IMenu, view) == 0x10);
}
