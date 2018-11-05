#pragma once

#include "skse64/GameEvents.h"  // EventDispatcher, MenuOpenCloseEvent, MenuModeChangeEvent
#include "skse64/GameMenus.h"  // MenuTableItem, MenuManager
#include "skse64/GameTypes.h"  // BSFixedString


namespace RE
{
	class GFxMovieView;
	class IMenu;


	class MenuManager
	{
	public:
		// parents
		void*									unk000;							// 000 - singleton
		EventDispatcher<MenuOpenCloseEvent>		menuOpenCloseEventDispatcher;	// 008
		EventDispatcher<MenuModeChangeEvent>	menuModeChangeEventDispatcher;	// 060
		EventDispatcher<void*>					unk0B8;							// 0B8 - New in 1.6.87.0 - Kinect related?


		typedef tHashSet<MenuTableItem, BSFixedString> MenuTable;
		typedef IMenu* (*CreatorFunc)(void);


		struct Unknown3
		{
			UInt64	unk00;		// 00 - New in SE. Not init'd?

			UInt32	freqLow;	// 08 (= Frequency.LowPart)
			UInt32	freqHigh;	// 0C (= Frequency.HighPart)

			UInt64	unk10;		// 10 (= 0)
			UInt32	unk18;		// 18 (= 0)
			UInt32	unk1C;		// 1C (= 0)
			UInt32	unk20;		// 20 (= 0)
			UInt32	unk24;		// 24 (= 0)
			float	unk28;		// 28 (= frequency related)

			UInt32	unk2C;		// 2C
			UInt32	unk30;		// 30

			UInt32	unk34;		// 34 (= 0)
			UInt16	unk38;		// 38
			UInt8	unk3A;		// 3A (= 0)
			UInt8	pad3B[5];	// 3B
		};


		inline static MenuManager*					GetSingleton(void)								{ return reinterpret_cast<MenuManager*>(::MenuManager::GetSingleton()); }
		inline EventDispatcher<MenuOpenCloseEvent>*	MenuOpenCloseEventDispatcher()					{ return reinterpret_cast<::MenuManager*>(this)->MenuOpenCloseEventDispatcher(); }
		inline bool									IsMenuOpen(BSFixedString* menuName)				{ return reinterpret_cast<::MenuManager*>(this)->IsMenuOpen(menuName); }
		inline IMenu*								GetMenu(BSFixedString* menuName)				{ return reinterpret_cast<IMenu*>(reinterpret_cast<::MenuManager*>(this)->GetMenu(menuName)); }
		inline GFxMovieView*						GetMovieView(BSFixedString* menuName)			{ return reinterpret_cast<GFxMovieView*>(reinterpret_cast<::MenuManager*>(this)->GetMovieView(menuName)); }
		inline void									ShowMenus(bool show)							{ reinterpret_cast<::MenuManager*>(this)->ShowMenus(show); }
		inline bool									IsShowingMenus()								{ return reinterpret_cast<::MenuManager*>(this)->IsShowingMenus(); }
		inline void									Register(const char* name, CreatorFunc creator)	{ reinterpret_cast<::MenuManager*>(this)->Register(name, reinterpret_cast<::MenuManager::CreatorFunc>(creator)); }

		inline bool									GameIsPaused()									{ return numPauseGame == 0; }


		// members
		UnkArray								menuStack;						// 110
		MenuTable								menuTable;						// 128   (Entries ptr at 150)
		SimpleLock								menuTableLock;					// 158
		UInt32									numPauseGame;					// 160 (= 0) += 1 if (imenu->flags & 0x0001)
		UInt32									numItemMenu;					// 164 (= 0) += 1 if (imenu->flags & 0x2000)
		UInt32									numPreventGameLoad;				// 168 (= 0) += 1 if (imenu->flags & 0x0080)
		UInt32									numDoNotPreventSaveGame;		// 16C (= 0) += 1 if (imenu->flags & 0x0800)
		UInt32									numStopCrosshairUpdate;			// 170 (= 0) += 1 if (imenu->flags & 0x4000)
		UInt32									numFlag8000;					// 174 (= 0) += 1 if (imenu->flags & 0x8000)
		UInt32									numFlag20000;					// 178 (= 0)  = 1 if (imenu->flags & 0x20000)
		UInt8									numModal;						// 17C (= 0)  = 1 if (imenu->flags & 0x10)
		UInt8									pad17D[3];						// 17D
		Unknown3								unk180;							// 180
		bool									showMenus;						// 1C0 (= 0)
		bool									unk1C1;							// 1C1 (= 0)
		char									pad1C2[6];						// 1C2
	};
}
