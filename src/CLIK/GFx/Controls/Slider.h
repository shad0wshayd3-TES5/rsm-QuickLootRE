#pragma once

#include "CLIK/GFx/Core/UIComponent.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class Slider : public Core::UIComponent
			{
			private:
				using super = Core::UIComponent;

			public:
				Slider() = default;
				Slider(const Slider&) = default;
				Slider(Slider&&) = default;
				using super::super;

				inline Slider(const super& a_rhs) :
					super(a_rhs)
				{}

				inline Slider(super&& a_rhs) :
					super(std::move(a_rhs))
				{}

				~Slider() = default;

				Slider& operator=(const Slider&) = default;
				Slider& operator=(Slider&&) = default;
				using super::operator=;

				inline Slider& operator=(const super& a_rhs)
				{
					super::operator=(a_rhs);
					return *this;
				}

				inline Slider& operator=(super&& a_rhs)
				{
					super::operator=(std::move(a_rhs));
					return *this;
				}

				inline double Maximum() const { return GetNumber("maximum"); }
				inline void Maximum(double a_maximum) { SetNumber("maximum", a_maximum); }

				inline double Minimum() const { return GetNumber("minimum"); }
				inline void Minimum(double a_minimum) { SetNumber("minimum", a_minimum); }

				inline double Value() const { return GetNumber("value"); }
				inline void Value(double a_value) { SetNumber("value", a_value); }

				inline bool Disabled() const { return GetBoolean("disabled"); }
				inline void Disabled(bool a_disabled) { SetBoolean("disabled", a_disabled); }

				inline double Position() const { return GetNumber("position"); }
				inline void Position(double a_position) { SetNumber("position", a_position); }

				inline bool Snapping() const { return GetBoolean("snapping"); }
				inline void Snapping(bool a_snapping) { SetBoolean("snapping", a_snapping); }

				inline double SnapInterval() const { return GetNumber("snapInterval"); }
				inline void SnapInterval(double a_snapInterval) { SetNumber("snapInterval", a_snapInterval); }

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				inline std::string ToString()
				{
					RE::GFxValue str;
					[[maybe_unused]] const auto success =
						Invoke("toString", std::addressof(str));
					assert(success);
					return str.GetString();
				}
			};
		}
	}
}
