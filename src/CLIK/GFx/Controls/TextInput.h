#pragma once

#include "CLIK/GFx/Core/UIComponent.h"

namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			class TextInput : public Core::UIComponent
			{
			public:
				using super = Core::UIComponent;

				TextInput();
				TextInput(const TextInput& a_rhs);
				TextInput(TextInput&& a_rhs);
				TextInput(const Core::UIComponent& a_rhs);
				TextInput(Core::UIComponent&& a_rhs);
				explicit TextInput(const RE::GFxValue& a_val);
				explicit TextInput(RE::GFxValue&& a_val);
				~TextInput();

				TextInput& operator=(const TextInput& a_rhs);
				TextInput& operator=(TextInput&& a_rhs);
				TextInput& operator=(const Core::UIComponent& a_rhs);
				TextInput& operator=(Core::UIComponent&& a_rhs);
				TextInput& operator=(const RE::GFxValue& a_rhs);
				TextInput& operator=(RE::GFxValue&& a_rhs);

				std::string_view TextID() const;
				void TextID(std::string_view a_textID);

				std::string_view Text() const;
				void Text(std::string_view a_text);

				std::string_view HTMLText() const;
				void HTMLText(std::string_view a_htmlText);

				bool Editable() const;
				void Editable(bool a_editable);

				bool Password() const;
				void Password(bool a_password);

				double MaxChars() const;
				void MaxChars(double a_maxChars);

				bool Disabled() const;
				void Disabled(bool a_disabled);

				void AppendText(std::string_view a_text);
				void AppendHTML(std::string_view a_text);

				double Length() const;

				//bool handleInput(InputDetails& a_details, Array& a_pathToFocus);

				std::string_view ToString();
			};
		}
	}
}
