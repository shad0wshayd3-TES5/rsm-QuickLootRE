#pragma once

#include "CLIK/UIComponent.h"


namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class Slider : public Core::UIComponent
			{
			public:
				using super = Core::UIComponent;


				Slider();
				Slider(const Slider& a_rhs);
				Slider(Slider&& a_rhs);
				Slider(const Core::UIComponent& a_rhs);
				Slider(Core::UIComponent&& a_rhs);
				explicit Slider(const RE::GFxValue& a_val);
				explicit Slider(RE::GFxValue&& a_val);
				~Slider();

				Slider& operator=(const Slider& a_rhs);
				Slider& operator=(Slider&& a_rhs);
				Slider& operator=(const Core::UIComponent& a_rhs);
				Slider& operator=(Core::UIComponent&& a_rhs);
				Slider& operator=(const RE::GFxValue& a_rhs);
				Slider& operator=(RE::GFxValue&& a_rhs);

				double Maximum() const;
				void Maximum(double a_maximum);

				double Minimum() const;
				void Minimum(double a_minimum);

				double Value() const;
				void Value(double a_value);

				bool Disabled() const;
				void Disabled(bool a_disabled);

				double Position() const;
				void Position(double a_position);

				bool Snapping() const;
				void Snapping(bool a_snapping);

				double SnapInterval() const;
				void SnapInterval(double a_snapInterval);

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				std::string_view ToString();
			};
		}
	}
}
