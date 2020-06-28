#pragma once

#include "CLIK/GFx/Controls/CoreList.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class ScrollingList : public CoreList
			{
			public:
				using super = CoreList;

				ScrollingList();
				ScrollingList(const ScrollingList& a_rhs);
				ScrollingList(ScrollingList&& a_rhs);
				ScrollingList(const CoreList& a_rhs);
				ScrollingList(CoreList&& a_rhs);
				explicit ScrollingList(const RE::GFxValue& a_val);
				explicit ScrollingList(RE::GFxValue&& a_val);
				~ScrollingList();

				ScrollingList& operator=(const ScrollingList& a_rhs);
				ScrollingList& operator=(ScrollingList&& a_rhs);
				ScrollingList& operator=(const CoreList& a_rhs);
				ScrollingList& operator=(CoreList&& a_rhs);
				ScrollingList& operator=(const RE::GFxValue& a_rhs);
				ScrollingList& operator=(RE::GFxValue&& a_rhs);

				Object ScrollBar() const;
				void ScrollBar(const Object& a_scrollBar);

				double RowHeight() const;
				void RowHeight(double a_rowHeight);

				double ScrollPosition() const;
				void ScrollPosition(double a_scrollPosition);

				double SelectedIndex() const;
				void SelectedIndex(double a_selectedIndex);

				bool Disabled() const;
				void Disabled(bool a_disabled);

				void ScrollToIndex(double a_index);

				double RowCount() const;
				void RowCount(double a_rowCount);

				void InvalidateData();

				//bool handleInput(InputDetails& a_details, Array& a_pathToFocus);

				double AvailableWidth() const;

				std::string_view ToString();
			};
		}
	}
}
