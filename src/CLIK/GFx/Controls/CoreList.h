#pragma once

#include "CLIK/GFx/Core/UIComponent.h"

namespace CLIK
{
	class Array;

	namespace GFx
	{
		namespace Controls
		{
			class CoreList :
				public Core::UIComponent
			{
			private:
				using super = Core::UIComponent;

			public:
				CoreList() = default;
				CoreList(const CoreList&) = default;
				CoreList(CoreList&&) = default;
				using super::super;

				inline CoreList(const super& a_rhs) :
					super(a_rhs)
				{}

				inline CoreList(super&& a_rhs) :
					super(std::move(a_rhs))
				{}

				~CoreList() = default;

				CoreList& operator=(const CoreList&) = default;
				CoreList& operator=(CoreList&&) = default;
				using super::operator=;

				inline CoreList& operator=(const super& a_rhs)
				{
					super::operator=(a_rhs);
					return *this;
				}

				inline CoreList& operator=(super&& a_rhs)
				{
					super::operator=(std::move(a_rhs));
					return *this;
				}

				inline std::string ItemRenderer() const { return GetString("itemRenderer"); }
				inline void ItemRenderer(std::string_view a_itemRenderer) { SetString("itemRenderer", a_itemRenderer); }

				inline Object DataProvider() const { return GetObject("dataProvider"); }
				inline void DataProvider(const Object& a_dataProvider) { SetObject("dataProvider", a_dataProvider); }

				inline double SelectedIndex() const { return GetNumber("selectedIndex"); }
				inline void SelectedIndex(double a_selectedIndex) { SetNumber("selectedIndex", a_selectedIndex); }

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

				inline std::string LabelField() const { return GetString("labelField"); }
				inline void LabelField(std::string_view a_labelField) { SetString("labelField", a_labelField); }

				//Function& LabelFunction() const;
				//void LabelFunction(Function& a_labelFunction);

				inline std::string ItemToLabel(Object& a_item)
				{
					enum
					{
						kItem,
						kNumArgs
					};

					std::array<RE::GFxValue, kNumArgs> args;

					args[kItem] = a_item.GetInstance();
					assert(args[kItem].IsObject());

					RE::GFxValue str;
					[[maybe_unused]] const auto success =
						Invoke("itemToLabel", std::addressof(str), args.data(), args.size());
					assert(success);

					return str.GetString();
				}

				inline void InvalidateData()
				{
					[[maybe_unused]] const auto success =
						Invoke("invalidateData");
					assert(success);
				}

				inline double AvailableWidth() const { return GetNumber("availableWidth"); }

				inline double AvailableHeight() const { return GetNumber("availableHeight"); }

				inline void SetRendererList(Array& a_value)
				{
					enum
					{
						kValue,
						kNumArgs
					};

					std::array<RE::GFxValue, kNumArgs> args;

					args[kValue] = a_value.GetInstance();
					assert(args[kValue].IsArray());

					[[maybe_unused]] const auto success =
						Invoke("setRendererList", nullptr, args.data(), args.size());
					assert(success);
				}

				inline std::string RendererInstanceName() const { return GetString("rendererInstanceName"); }
				inline void RendererInstanceName(std::string_view a_rendererInstanceName) { SetString("rendererInstanceName", a_rendererInstanceName); }

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
