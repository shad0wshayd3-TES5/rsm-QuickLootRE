#include "RE/GFxLoader.h"

#include "skse64/ScaleformLoader.h"  // GFxLoader


namespace RE
{
	GFxLoader* GFxLoader::GetSingleton()
	{
		return reinterpret_cast<GFxLoader*>(::GFxLoader::GetSingleton());
	}


	bool GFxLoader::LoadMovie(IMenu* menu, GFxMovieView*& viewOut, const char* swfName, GFxMovieView::ScaleModeType mode, float backGroundAlpha)
	{
		::GFxMovieView* view = reinterpret_cast<::GFxMovieView*>(viewOut);
		bool result = CALL_MEMBER_FN(reinterpret_cast<::GFxLoader*>(this), LoadMovie)(reinterpret_cast<::IMenu*>(menu), &view, swfName, mode, backGroundAlpha);
		viewOut = reinterpret_cast<GFxMovieView*>(view);
		return result;
	}
}
