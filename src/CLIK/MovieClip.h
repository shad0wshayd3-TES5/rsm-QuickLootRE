#pragma once

#include "Object.h"

namespace CLIK
{
	class MovieClip : public Object
	{
	public:
		using super = Object;

		MovieClip();
		MovieClip(const MovieClip& a_rhs);
		MovieClip(MovieClip&& a_rhs);
		MovieClip(const Object& a_rhs);
		MovieClip(Object&& a_rhs);
		explicit MovieClip(const RE::GFxValue& a_val);
		explicit MovieClip(RE::GFxValue&& a_val);
		~MovieClip();

		MovieClip& operator=(const MovieClip& a_rhs);
		MovieClip& operator=(MovieClip&& a_rhs);
		MovieClip& operator=(const Object& a_rhs);
		MovieClip& operator=(Object&& a_rhs);
		MovieClip& operator=(const RE::GFxValue& a_rhs);
		MovieClip& operator=(RE::GFxValue&& a_rhs);

		// properties
		double Alpha() const;
		void Alpha(double a_alpha);

		Object BlendMode() const;
		void BlendMode(const Object& a_blendMode);

		bool CacheAsBitmap() const;
		void CacheAsBitmap(bool a_cacheAsBitmap);

		double Currentframe() const;

		std::string_view Droptarget() const;

		bool Enabled() const;
		void Enabled(bool a_enabled);

		//Array Filters() const;
		//void Filters(const Array& a_filters);

		bool FocusEnabled() const;
		void FocusEnabled(bool a_focusEnabled);

		bool Focusrect() const;
		void Focusrect(bool a_focusrect);

		double Framesloaded() const;

		double Height() const;
		void Height(double a_height);

		double HighQuality() const;
		void HighQuality(double a_highQuality);

		Object HitArea() const;
		void HitArea(const Object& a_hitArea);

		double LockRoot() const;
		void LockRoot(double a_lockRoot);

		//ContextMenu Menu() const;
		//void Menu(const ContextMenu& a_menu);

		std::string_view Name() const;
		void Name(std::string_view a_name);

		double OpaqueBackground() const;
		void OpaqueBackground(double a_opaqueBackground);

		MovieClip Parent() const;
		void Parent(const MovieClip& a_parent);

		std::string_view Quality() const;
		void Quality(std::string_view a_quality);

		double Rotation() const;
		void Rotation(double a_rotation);

		//Rectangle Scale9Grid() const;
		//void Scale9Grid(const Rectangle& a_scale9Grid) const;

		Object ScrollRect() const;
		void ScrollRect(const Object& a_scrollRect);

		double SoundBufTime() const;
		void SoundBufTime(double a_soundBufTime);

		bool TabChildren() const;
		void TabChildren(bool a_tabChildren);

		bool TabEnabled() const;
		void TabEnabled(bool a_tabEnabled);

		double TabIndex() const;
		void TabIndex(double a_tabIndex);

		std::string_view Target() const;

		double TotalFrames() const;

		bool TrackAsMenu() const;
		void TrackAsMenu(bool a_trackAsMenu);

		//Transform Transform() const;
		//void Transform(const Transform& a_transform);

		std::string_view URL() const;

		bool UseHandCursor() const;
		void UseHandCursor(bool a_useHandCursor);

		bool Visible() const;
		void Visible(bool a_visible);

		double Width() const;
		void Width(double a_width);

		double X() const;
		void X(double a_x);

		double XMouse() const;

		double XScale() const;
		void XScale(double a_xScale);

		double Y() const;
		void Y(double a_y);

		double YMouse() const;

		double YScale() const;
		void YScale(double a_yScale);
	};
}
