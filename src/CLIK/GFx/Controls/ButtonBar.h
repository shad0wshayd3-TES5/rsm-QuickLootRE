#pragma once

#include "CLIK/GFx/Core/UIComponent.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class ButtonBar :
				public Core::UIComponent
			{
			private:
				using super = Core::UIComponent;

			public:
				ButtonBar() = default;
				ButtonBar(const ButtonBar&) = default;
				ButtonBar(ButtonBar&&) = default;
				using super::super;

				inline ButtonBar(const super& a_rhs) :
					super(a_rhs)
				{}

				inline ButtonBar(super&& a_rhs) :
					super(std::move(a_rhs))
				{}

				~ButtonBar() = default;

				ButtonBar& operator=(const ButtonBar&) = default;
				ButtonBar& operator=(ButtonBar&&) = default;
				using super::operator=;

				inline ButtonBar& operator=(const super& a_rhs)
				{
					super::operator=(a_rhs);
					return *this;
				}

				inline ButtonBar& operator=(super&& a_rhs)
				{
					super::operator=(std::move(a_rhs));
					return *this;
				}

				inline bool Disabled() { return GetBoolean("disabled"); }
				inline void Disabled(bool a_value) { return SetBoolean("disabled", a_value); }

				inline Object DataProvider() { return GetObject("dataProvider"); }
				inline void DataProvider(const Object& a_value) { return SetObject("dataProvider", a_value); }

				inline void InvalidateData()
				{
					[[maybe_unused]] const auto success =
						Invoke("invalidateData");
					assert(success);
				}

				inline std::string ItemRenderer() { return GetString("itemRenderer"); }
				inline void ItemRenderer(std::string_view a_value) { return SetString("itemRenderer", std::move(a_value)); }

				inline double Spacing() { return GetNumber("spacing"); }
				inline void Spacing(double a_value) { return SetNumber("spacing", a_value); }

				inline std::string Direction() { return GetString("direction"); }
				inline void Direction(std::string_view a_value) { return SetString("direction", a_value); }

				inline std::string AutoSize() { return GetString("autoSize"); }
				inline void AutoSize(std::string_view a_value) { return SetString("autoSize", a_value); }

				inline double ButtonWidth() { return GetNumber("buttonWidth"); }
				inline void ButtonWidth(double a_value) { return SetNumber("buttonWidth", a_value); }

				inline double SelectedIndex() { return GetNumber("selectedIndex"); }
				inline void SelectedIndex(double a_value) { return SetNumber("selectedIndex", a_value); }

				inline Object SelectedItem() { return GetObject("selectedItem"); }

				inline Object Data() { return GetObject("data"); }

				inline std::string LabelField() { return GetString("labelField"); }
				inline void LabelField(std::string_view a_value) { return SetString("labelField", a_value); }

				//inline CLIK::Function LabelFunction() { return GetObject("labelFunction"); }
				//inline void LabelFunction(CLIK::Function a_value) { return SetObject("labelFunction", a_value); }

				inline std::string ItemToLabel(Object a_item)
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

				//bool handleInput(CLIK::GFx::UI::InputDetails a_details, CLIK::Array a_pathToFocus);

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
