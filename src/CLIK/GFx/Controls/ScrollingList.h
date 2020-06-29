#pragma once

#include "CLIK/GFx/Controls/CoreList.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class ScrollingList :
				public CoreList
			{
			private:
				using super = CoreList;

			public:
				ScrollingList() = default;
				ScrollingList(const ScrollingList&) = default;
				ScrollingList(ScrollingList&&) = default;
				using super::super;

				inline ScrollingList(const super& a_rhs) :
					super(a_rhs)
				{}

				inline ScrollingList(super&& a_rhs) :
					super(std::move(a_rhs))
				{}

				~ScrollingList() = default;

				ScrollingList& operator=(const ScrollingList&) = default;
				ScrollingList& operator=(ScrollingList&&) = default;
				using super::operator=;

				inline ScrollingList& operator=(const super& a_rhs)
				{
					super::operator=(a_rhs);
					return *this;
				}

				inline ScrollingList& operator=(super&& a_rhs)
				{
					super::operator=(std::move(a_rhs));
					return *this;
				}

				inline Object ScrollBar() const { return GetObject("scrollBar"); }
				inline void ScrollBar(const Object& a_scrollBar) { SetObject("scrollBar", a_scrollBar); }

				inline double RowHeight() const { return GetNumber("rowHeight"); }
				inline void RowHeight(double a_rowHeight) { SetNumber("rowHeight", a_rowHeight); }

				inline double ScrollPosition() const { return GetNumber("scrollPosition"); }
				inline void ScrollPosition(double a_scrollPosition) { SetNumber("scrollPosition", a_scrollPosition); }

				inline double SelectedIndex() const { return GetNumber("selectedIndex"); }
				inline void SelectedIndex(double a_selectedIndex) { SetNumber("selectedIndex", a_selectedIndex); }

				inline bool Disabled() const { return GetBoolean("disabled"); }
				inline void Disabled(bool a_disabled) { SetBoolean("disabled", a_disabled); }

				inline void ScrollToIndex(double a_index)
				{
					enum
					{
						kIndex,
						kNumArgs
					};

					std::array<RE::GFxValue, kNumArgs> args;

					args[kIndex] = a_index;
					assert(args[kIndex].IsNumber());

					[[maybe_unused]] const auto success =
						Invoke("scrollToIndex", nullptr, args.data(), args.size());
					assert(success);
				}

				inline double RowCount() const { return GetNumber("rowCount"); }
				inline void RowCount(double a_rowCount) { SetNumber("rowCount", a_rowCount); }

				inline void InvalidateData()
				{
					[[maybe_unused]] const auto success =
						Invoke("invalidateData");
					assert(success);
				}

				//bool handleInput(InputDetails& a_details, Array& a_pathToFocus);

				inline double AvailableWidth() const { return GetNumber("availableWidth"); }

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
