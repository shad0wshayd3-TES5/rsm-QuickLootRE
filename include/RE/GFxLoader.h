#pragma once

#include "RE/GFxMovieView.h"  // RE::GFxMovieView::ScaleModeType


namespace RE
{
	class BSScaleformImageLoader;
	class GFxStateBag;
	class IMenu;


	class GFxLoader
	{
	public:
		static GFxLoader*	GetSingleton();

		bool				LoadMovie(IMenu* menu, GFxMovieView*& viewOut, const char* swfName, GFxMovieView::ScaleModeType mode, float backGroundAlpha);


		// members
		UInt32					unk00;			// 00
		GFxStateBag*			stateBag;		// 04
		UInt32					unk08;			// 08
		UInt32					unk0C;			// 0C
		BSScaleformImageLoader*	imageLoader;	// 10
	};
}
