#pragma once

#include "CLIK/UIComponent.h"


namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class Button : public Core::UIComponent
			{
			public:
				using super = Core::UIComponent;


				Button();
				Button(const Button& a_rhs);
				Button(Button&& a_rhs);
				Button(const Core::UIComponent& a_rhs);
				Button(Core::UIComponent&& a_rhs);
				explicit Button(const RE::GFxValue& a_val);
				explicit Button(RE::GFxValue&& a_val);
				~Button();

				Button& operator=(const Button& a_rhs);
				Button& operator=(Button&& a_rhs);
				Button& operator=(const Core::UIComponent& a_rhs);
				Button& operator=(Core::UIComponent&& a_rhs);
				Button& operator=(const RE::GFxValue& a_rhs);
				Button& operator=(RE::GFxValue&& a_rhs);

				std::string_view LabelID() const;
				void LabelID(std::string_view a_labelID);

				std::string_view Label() const;
				void Label(std::string_view a_label);

				bool Disabled() const;
				void Disabled(bool a_disabled);

				bool Selected() const;
				void Selected(bool a_selected);

				std::string_view GroupName() const;
				void GroupName(std::string_view a_groupName);

				Object Group() const;
				void Group(const Object& a_group);

				bool DisableFocus() const;
				void DisableFocus(bool a_disableFocus);

				bool DisableConstraints() const;
				void DisableConstraints(bool a_disableConstraints);

				std::string_view AutoSize() const;
				void AutoSize(std::string_view a_autoSize);

				void SetSize(double a_width, double a_height);

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				std::string_view ToString();
			};
		}
	}
}
