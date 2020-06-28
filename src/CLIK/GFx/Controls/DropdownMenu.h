#pragma once

#include "CLIK/GFx/Controls/Button.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class DropdownMenu : public Button
			{
			public:
				using super = Button;

				DropdownMenu();
				DropdownMenu(const DropdownMenu& a_rhs);
				DropdownMenu(DropdownMenu&& a_rhs);
				DropdownMenu(const Button& a_rhs);
				DropdownMenu(Button&& a_rhs);
				explicit DropdownMenu(const RE::GFxValue& a_val);
				explicit DropdownMenu(RE::GFxValue&& a_val);
				~DropdownMenu();

				DropdownMenu& operator=(const DropdownMenu& a_rhs);
				DropdownMenu& operator=(DropdownMenu&& a_rhs);
				DropdownMenu& operator=(const Button& a_rhs);
				DropdownMenu& operator=(Button&& a_rhs);
				DropdownMenu& operator=(const RE::GFxValue& a_rhs);
				DropdownMenu& operator=(RE::GFxValue&& a_rhs);

				Object Dropdown() const;
				void Dropdown(const Object& a_dropdown);

				Object ItemRenderer() const;
				void ItemRenderer(const Object& a_itemRenderer);

				Object ScrollBar() const;
				void ScrollBar(const Object& a_scrollBar);

				Object DropdownWidth() const;
				void DropdownWidth(const Object& a_dropdownWidth);

				double RowCount() const;
				void RowCount(double a_rowCount);

				Object DataProvider() const;
				void DataProvider(const Object& a_dataProvider);

				double SelectedIndex() const;
				void SelectedIndex(double a_selectedIndex);

				std::string_view LabelField() const;
				void LabelField(std::string_view a_selectedIndex);

				//Function LabelFunction() const;
				//void LabelFunction(const Function& a_labelFunction);

				std::string_view ItemToLabel(Object& a_item);

				void Open();
				void Close();

				void InvalidateData();

				void SetSize(double a_width, double a_height);

				//bool HandleInput(InputDetails& a_details, Array& a_pathToFocus);

				void RemoveMovieClip();

				std::string_view ToString();
			};
		}
	}
}
