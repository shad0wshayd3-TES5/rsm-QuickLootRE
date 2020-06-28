#pragma once

#include "CLIK/MovieClip.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Core
		{
			class UIComponent : public MovieClip
			{
			public:
				using super = MovieClip;

				UIComponent();
				UIComponent(const UIComponent& a_rhs);
				UIComponent(UIComponent&& a_rhs);
				UIComponent(const MovieClip& a_rhs);
				UIComponent(MovieClip&& a_rhs);
				explicit UIComponent(const RE::GFxValue& a_val);
				explicit UIComponent(RE::GFxValue&& a_val);
				~UIComponent();

				UIComponent& operator=(const UIComponent& a_rhs);
				UIComponent& operator=(UIComponent&& a_rhs);
				UIComponent& operator=(const MovieClip& a_rhs);
				UIComponent& operator=(MovieClip&& a_rhs);
				UIComponent& operator=(const RE::GFxValue& a_rhs);
				UIComponent& operator=(RE::GFxValue&& a_rhs);

				bool Disabled() const;
				void Disabled(bool a_disabled);

				bool Visible() const;
				void Visible(bool a_visible);

				double Width() const;
				void Width(double a_width);

				double Height() const;
				void Height(double a_height);

				void SetSize(double a_width, double a_height);

				double Focused() const;
				void Focused(double a_focused);

				bool DisplayFocus() const;
				void DisplayFocus(bool a_displayFocus);

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				void Invalidate();
				void ValidateNow();

				std::string_view ToString();

				void DispatchEventToGame(Object& a_event);
			};
		}
	}
}
