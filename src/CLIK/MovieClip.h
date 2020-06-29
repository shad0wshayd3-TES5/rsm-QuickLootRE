#pragma once

#include "Object.h"

namespace CLIK
{
	class MovieClip :
		public Object
	{
	private:
		using super = Object;

	public:
		MovieClip() = default;
		MovieClip(const MovieClip&) = default;
		MovieClip(MovieClip&&) = default;
		using super::super;

		inline MovieClip(const super& a_rhs) :
			super(a_rhs)
		{}

		inline MovieClip(super&& a_rhs) :
			super(std::move(a_rhs))
		{}

		~MovieClip() = default;

		MovieClip& operator=(const MovieClip&) = default;
		MovieClip& operator=(MovieClip&&) = default;
		using super::operator=;

		inline MovieClip& operator=(const super& a_rhs)
		{
			super::operator=(a_rhs);
			return *this;
		}

		inline MovieClip& operator=(super&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			return *this;
		}

		// properties
		inline double Alpha() const { return GetNumber("_alpha"); }
		inline void Alpha(double a_alpha) { SetNumber("_alpha", a_alpha); }

		inline Object BlendMode() const { return GetObject("blendMode"); }
		inline void BlendMode(const Object& a_blendMode) { SetObject("blendMode", a_blendMode); }

		inline bool CacheAsBitmap() const { return GetBoolean("cacheAsBitmap"); }
		inline void CacheAsBitmap(bool a_cacheAsBitmap) { SetBoolean("cacheAsBitmap", a_cacheAsBitmap); }

		inline double Currentframe() const { return GetNumber("_currentframe"); }

		inline std::string_view Droptarget() const { return GetString("_droptarget"); }

		inline bool Enabled() const { return GetBoolean("enabled"); }
		inline void Enabled(bool a_enabled) { SetBoolean("enabled", a_enabled); }

		//Array Filters() const;
		//void Filters(const Array& a_filters);

		inline bool FocusEnabled() const { return GetBoolean("focusEnabled"); }
		inline void FocusEnabled(bool a_focusEnabled) { SetBoolean("focusEnabled", a_focusEnabled); }

		inline bool Focusrect() const { return GetBoolean("_focusrect"); }
		inline void Focusrect(bool a_focusrect) { SetBoolean("_focusrect", a_focusrect); }

		inline double Framesloaded() const { return GetNumber("_framesloaded"); }

		inline double Height() const { return GetNumber("_height"); }
		inline void Height(double a_height) { SetNumber("_height", a_height); }

		inline double HighQuality() const { return GetNumber("_highquality"); }
		inline void HighQuality(double a_highQuality) { SetNumber("_highquality", a_highQuality); }

		inline Object HitArea() const { return GetObject("hitArea"); }
		inline void HitArea(const Object& a_hitArea) { SetObject("hitArea", a_hitArea); }

		inline double LockRoot() const { return GetNumber("_lockroot"); }
		inline void LockRoot(double a_lockRoot) { SetNumber("_lockroot", a_lockRoot); }

		//ContextMenu Menu() const;
		//void Menu(const ContextMenu& a_menu);

		inline std::string_view Name() const { return GetString("_name"); }
		inline void Name(std::string_view a_name) { SetString("_name", a_name); }

		inline double OpaqueBackground() const { return GetNumber("opaqueBackground"); }
		inline void OpaqueBackground(double a_opaqueBackground) { SetNumber("opaqueBackground", a_opaqueBackground); }

		inline MovieClip Parent() const { return GetObject("_parent"); }
		inline void Parent(const MovieClip& a_parent) { SetObject("_parent", a_parent); }

		inline std::string_view Quality() const { return GetString("_quality"); }
		inline void Quality(std::string_view a_quality) { SetString("_quality", a_quality); }

		inline double Rotation() const { return GetNumber("_rotation"); }
		inline void Rotation(double a_rotation) { SetNumber("_rotation", a_rotation); }

		//Rectangle Scale9Grid() const;
		//void Scale9Grid(const Rectangle& a_scale9Grid) const;

		inline Object ScrollRect() const { return GetObject("scrollRect"); }
		inline void ScrollRect(const Object& a_scrollRect) { SetObject("scrollRect", a_scrollRect); }

		inline double SoundBufTime() const { return GetNumber("_soundbuftime"); }
		inline void SoundBufTime(double a_soundBufTime) { SetNumber("_soundbuftime", a_soundBufTime); }

		inline bool TabChildren() const { return GetBoolean("tabChildren"); }
		inline void TabChildren(bool a_tabChildren) { SetBoolean("tabChildren", a_tabChildren); }

		inline bool TabEnabled() const { return GetBoolean("tabEnabled"); }
		inline void TabEnabled(bool a_tabEnabled) { SetBoolean("tabEnabled", a_tabEnabled); }

		inline double TabIndex() const { return GetNumber("tabIndex"); }
		inline void TabIndex(double a_tabIndex) { SetNumber("tabIndex", a_tabIndex); }

		inline std::string_view Target() const { return GetString("_target"); }

		inline double TotalFrames() const { return GetNumber("_totalframes"); }

		inline bool TrackAsMenu() const { return GetBoolean("trackAsMenu"); }
		inline void TrackAsMenu(bool a_trackAsMenu) { SetBoolean("trackAsMenu", a_trackAsMenu); }

		//Transform Transform() const;
		//void Transform(const Transform& a_transform);

		inline std::string_view URL() const { return GetString("_url"); }

		inline bool UseHandCursor() const { return GetBoolean("useHandCursor"); }
		inline void UseHandCursor(bool a_useHandCursor) { SetBoolean("useHandCursor", a_useHandCursor); }

		inline bool Visible() const { return GetBoolean("_visible"); }
		inline void Visible(bool a_visible) { SetBoolean("_visible", a_visible); }

		inline double Width() const { return GetNumber("_width"); }
		inline void Width(double a_width) { SetNumber("_width", a_width); }

		inline double X() const { return GetNumber("_x"); }
		inline void X(double a_x) { SetNumber("_x", a_x); }

		inline double XMouse() const { return GetNumber("_xmouse"); }

		inline double XScale() const { return GetNumber("_xscale"); }
		inline void XScale(double a_xScale) { SetNumber("_xscale", a_xScale); }

		inline double Y() const { return GetNumber("_y"); }
		inline void Y(double a_y) { SetNumber("_y", a_y); }

		inline double YMouse() const { return GetNumber("_ymouse"); }

		inline double YScale() const { return GetNumber("_yscale"); }
		inline void YScale(double a_yScale) { SetNumber("_yscale", a_yScale); }
	};
}
