#pragma once

#include "CLIK/GFx/Core/UIComponent.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class Button :
				public Core::UIComponent
			{
			private:
				using super = Core::UIComponent;

			public:
				Button() = default;
				Button(const Button&) = default;
				Button(Button&&) = default;
				using super::super;

				inline Button(const super& a_rhs) :
					super(a_rhs)
				{}

				inline Button(super&& a_rhs) :
					super(std::move(a_rhs))
				{}

				~Button() = default;

				Button& operator=(const Button&) = default;
				Button& operator=(Button&&) = default;
				using super::operator=;

				inline Button& operator=(const super& a_rhs)
				{
					super::operator=(a_rhs);
					return *this;
				}

				inline Button& operator=(super&& a_rhs)
				{
					super::operator=(std::move(a_rhs));
					return *this;
				}

				inline std::string_view LabelID() const { return GetString("labelID"); }
				inline void LabelID(std::string_view a_labelID) { SetString("labelID", a_labelID); }

				inline std::string_view Label() const { return GetString("label"); }
				inline void Label(std::string_view a_label) { SetString("label", a_label); }

				inline bool Disabled() const { return GetBoolean("disabled"); }
				inline void Disabled(bool a_disabled) { SetBoolean("disabled", a_disabled); }

				inline bool Selected() const { return GetBoolean("selected"); }
				inline void Selected(bool a_selected) { SetBoolean("selected", a_selected); }

				inline std::string_view GroupName() const { return GetString("groupName"); }
				inline void GroupName(std::string_view a_groupName) { SetString("groupName", a_groupName); }

				inline Object Group() const { return GetObject("group"); }
				inline void Group(const Object& a_group) { SetObject("group", a_group); }

				inline bool DisableFocus() const { return GetBoolean("disableFocus"); }
				inline void DisableFocus(bool a_disableFocus) { SetBoolean("disableFocus", a_disableFocus); }

				inline bool DisableConstraints() const { return GetBoolean("disableConstraints"); }
				inline void DisableConstraints(bool a_disableConstraints) { SetBoolean("disableConstraints", a_disableConstraints); }

				inline std::string_view AutoSize() const { return GetString("autoSize"); }
				inline void AutoSize(std::string_view a_autoSize) { SetString("autoSize", a_autoSize); }

				inline void SetSize(double a_width, double a_height)
				{
					enum
					{
						kWidth,
						kHeight,
						kNumArgs
					};

					std::array<RE::GFxValue, kNumArgs> args;

					args[kWidth] = a_width;
					assert(args[kWidth].IsNumber());

					args[kHeight] = a_height;
					assert(args[kHeight].IsNumber());

					[[maybe_unused]] const auto success =
						Invoke("setSize", nullptr, args.data(), args.size());
					assert(success);
				}

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				inline std::string_view ToString()
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
