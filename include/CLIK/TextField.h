#pragma once

#include "CLIK/Object.h"


namespace CLIK
{
	class TextField : public Object
	{
	public:
		using super = Object;


		TextField();
		TextField(const TextField& a_rhs);
		TextField(TextField&& a_rhs);
		TextField(const Object& a_rhs);
		TextField(Object&& a_rhs);
		explicit TextField(const RE::GFxValue& a_val);
		explicit TextField(RE::GFxValue&& a_val);
		~TextField();

		TextField& operator=(const TextField& a_rhs);
		TextField& operator=(TextField&& a_rhs);
		TextField& operator=(const Object& a_rhs);
		TextField& operator=(Object&& a_rhs);
		TextField& operator=(const RE::GFxValue& a_rhs);
		TextField& operator=(RE::GFxValue&& a_rhs);

		// properties
		double Alpha() const;
		void Alpha(double a_alpha);

		std::string_view AntiAliasType() const;
		void AntiAliasType(std::string_view a_antiAliasType);

		Object AutoSize() const;
		void AutoSize(const Object& a_autoSize);

		bool Background() const;
		void Background(bool a_background);

		double BackgroundColor() const;
		void BackgroundColor(double a_backgroundColor);

		bool Border() const;
		void Border(bool a_border);

		double BorderColor() const;
		void BorderColor(double a_borderColor);

		double BottomScroll() const;

		bool CondenseWhite() const;
		void CondenseWhite(bool a_condenseWhite);

		bool EmbedFonts() const;
		void EmbedFonts(bool a_embedFonts);

		//Array Filters() const;
		//void Filters(Array& a_filters);

		std::string_view GridFitType() const;
		void GridFitType(std::string_view a_gridFitType);

		double Height() const;
		void Height(double a_height);

		double HighQuality() const;
		void HighQuality(double a_highQuality);

		double HScroll() const;
		void HScroll(double a_hscroll);

		bool HTML() const;
		void HTML(bool a_html);

		std::string_view HTMLText() const;
		void HTMLText(std::string_view a_htmlText);

		double Length() const;

		double MaxChars() const;
		void MaxChars(double a_maxChars);

		double MaxHScroll() const;

		double MaxScroll() const;

		//ContextMenu Menu() const;

		bool MouseWheelEnabled() const;
		void MouseWheelEnabled(bool a_mouseWheelEnabled);

		bool Multiline() const;
		void Multiline(bool a_multiline);

		std::string_view Name() const;
		void Name(std::string_view a_name);

		//MovieClip Parent() const;
		//void Parent(const MovieClip& a_parent);

		bool Password() const;
		void Password(bool a_password);

		std::string_view Quality() const;
		void Quality(std::string_view a_quality);

		std::string_view Restrict() const;
		void Restrict(std::string_view a_restrict);

		double Rotation() const;
		void Rotation(double a_rotation);

		double Scroll() const;
		void Scroll(double a_scroll);

		bool Selectable() const;
		void Selectable(bool a_selectable);

		double Sharpness() const;
		void Sharpness(double a_sharpness);

		double SoundBufTime() const;
		void SoundBufTime(double a_soundBufTime);

		//StyleSheet StyleSheet() const;
		//void StyleSheet(const StyleSheet& a_styleSheet);

		bool TabEnabled() const;
		void TabEnabled(bool a_tabEnabled);

		double TabIndex() const;
		void TabIndex(double a_tabIndex);

		std::string_view Target() const;

		std::string_view Text() const;
		void Text(std::string_view a_text);

		double TextColor() const;
		void TextColor(double a_textColor);

		double TextHeight() const;
		void TextHeight(double a_textHeight);

		double TextWidth() const;
		void TextWidth(double a_textWidth);

		double Thickness() const;
		void Thickness(double a_thickness);

		std::string_view Type() const;
		void Type(std::string_view a_type);

		std::string_view URL() const;

		std::string_view Variable() const;
		void Variable(std::string_view a_variable);

		bool Visible() const;
		void Visible(bool a_visible);

		double Width() const;
		void Width(double a_width);

		bool WordWrap() const;
		void WordWrap(bool a_wordWrap);

		double X() const;
		void X(double a_x);

		double XMouse() const;

		double XScale() const;
		void XScale(double a_xScale);

		double Y() const;
		void Y(double a_y);

		double YMouse() const;

		double YScale() const;
		void YScale(double a_yScale);

		// methods
		bool AddListener(Object& a_listener);

		double GetDepth();

		//TextFormat GetNewTextFormat();
		//TextFormat GetTextFormat(std::optional<double> a_beginIndex, std::optional<double> a_endIndex);

		bool RemoveListener(Object& a_listener);

		void RemoveTextField();

		void ReplaceSel(std::string_view a_newText);
		void ReplaceText(double a_beginIndex, double a_endIndex, std::string_view a_newText);

		//void SetNewTextFormat(TextFormat& a_tf);

		//void SetTextFormat(std::optional<double> a_beginIndex, std::optional<double> a_endIndex, TextFormat& a_textFormat);;

		// gfx properties
		bool AutoFit() const;
		void AutoFit(bool a_autoFit);

		double CaretIndex() const;
		void CaretIndex(double a_caretIndex);

		double FocusGroup() const;
		void FocusGroup(double a_focusGroup);

		bool HitTestDisable() const;
		void HitTestDisable(bool a_hitTestDisable);

		bool NoTranslate() const;
		void NoTranslate(bool a_noTranslate);

		double NumLines() const;
		void NumLines(double a_numLines);

		bool TopmostLevel() const;
		void TopmostLevel(bool a_topmostLevel);

		double InactiveSelectionBkgColor() const;
		void InactiveSelectionBkgColor(double a_inactiveSelectionBkgColor);

		bool AlwaysShowSelection() const;
		void AlwaysShowSelection(bool a_alwaysShowSelection);

		bool NoAutoSelection() const;
		void NoAutoSelection(bool a_noAutoSelection);

		double SelectionBeginIndex() const;
		void SelectionBeginIndex(double a_selectionBeginIndex);

		double SelectionEndIndex() const;
		void SelectionEndIndex(double a_selectionEndIndex);

		double SelectionBkgColor() const;
		void SelectionBkgColor(double a_selectionBkgColor);

		double SelectionTextColor() const;
		void SelectionTextColor(double a_selectionTextColor);

		bool UseRichTextClipboard() const;
		void UseRichTextClipboard(bool a_useRichTextClipboard);

		double InactiveSelectionTextColor() const;
		void InactiveSelectionTextColor(double a_inactiveSelectionTextColor);

		double FontScaleFactor() const;
		void FontScaleFactor(double a_fontScaleFactor);

		std::string_view TextAutoSize() const;
		void TextAutoSize(std::string_view a_textAutoSize);

		std::string_view VerticalAlign() const;
		void VerticalAlign(std::string_view a_verticalAlign);

		std::string_view VerticalAutoSize() const;
		void VerticalAutoSize(std::string_view a_verticalAutoSize);

		// gfx methods
		void AppendText(std::string_view a_newText);
		void AppendHtml(std::string_view a_newHtml);

		//Rectangle GetCharBoundaries(double a_charIndex);
		//Rectangle GetExactCharBoundaries(double a_charIndex);

		double GetCharIndexAtPoint(double a_x, double a_y);

		double GetFirstCharInParagraph(double a_charIndex);

		double GetLineIndexAtPoint(double a_x, double a_y);
		double GetLineLength(double a_lineIndex);
		Object GetLineMetrics(double a_lineIndex);
		double GetLineOffset(double a_lineIndex);
		std::string_view GetLineText(double a_lineIndex);

		void CopyToClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex);
		void CutToClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex);
		void PasteFromClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex);
	};
}
