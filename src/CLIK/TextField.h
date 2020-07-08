#pragma once

#include "CLIK/Object.h"

namespace CLIK
{
	class TextField :
		public Object
	{
	private:
		using super = Object;

	public:
		TextField() = default;
		TextField(const TextField&) = default;
		TextField(TextField&&) = default;
		using super::super;

		inline TextField(const super& a_rhs) :
			super(a_rhs)
		{}

		inline TextField(super&& a_rhs) :
			super(std::move(a_rhs))
		{}

		~TextField() = default;

		TextField& operator=(const TextField&) = default;
		TextField& operator=(TextField&&) = default;
		using super::operator=;

		inline TextField& operator=(const super& a_rhs)
		{
			super::operator=(a_rhs);
			return *this;
		}

		inline TextField& operator=(super&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			return *this;
		}

		// properties
		inline double Alpha() const { return GetNumber("_alpha"); }
		inline void Alpha(double a_alpha) { SetNumber("_alpha", a_alpha); }

		inline std::string AntiAliasType() const { return GetString("antiAliasType"); }
		inline void AntiAliasType(std::string_view a_antiAliasType) { SetString("antiAliasType", a_antiAliasType); }

		inline Object AutoSize() const { return GetObject("autoSize"); }
		inline void AutoSize(const Object& a_autoSize) { SetObject("autoSize", a_autoSize); }

		inline bool Background() const { return GetBoolean("background"); }
		inline void Background(bool a_background) { SetBoolean("background", a_background); }

		inline double BackgroundColor() const { return GetNumber("backgroundColor"); }
		inline void BackgroundColor(double a_backgroundColor) { SetNumber("backgroundColor", a_backgroundColor); }

		inline bool Border() const { return GetBoolean("border"); }
		inline void Border(bool a_border) { SetBoolean("border", a_border); }

		inline double BorderColor() const { return GetNumber("borderColor"); }
		inline void BorderColor(double a_borderColor) { SetNumber("borderColor", a_borderColor); }

		inline double BottomScroll() const { return GetNumber("bottomScroll"); }

		inline bool CondenseWhite() const { return GetBoolean("condenseWhite"); }
		inline void CondenseWhite(bool a_condenseWhite) { SetBoolean("condenseWhite", a_condenseWhite); }

		inline bool EmbedFonts() const { return GetBoolean("embedFonts"); }
		inline void EmbedFonts(bool a_embedFonts) { SetBoolean("embedFonts", a_embedFonts); }

		//Array Filters() const;
		//void Filters(Array& a_filters);

		inline std::string GridFitType() const { return GetString("gridFitType"); }
		inline void GridFitType(std::string_view a_gridFitType) { SetString("gridFitType", a_gridFitType); }

		inline double Height() const { return GetNumber("_height"); }
		inline void Height(double a_height) { SetNumber("_height", a_height); }

		inline double HighQuality() const { return GetNumber("_highquality"); }
		inline void HighQuality(double a_highQuality) { SetNumber("_highquality", a_highQuality); }

		inline double HScroll() const { return GetNumber("hscroll"); }
		inline void HScroll(double a_hscroll) { SetNumber("hscroll", a_hscroll); }

		inline bool HTML() const { return GetBoolean("html"); }
		inline void HTML(bool a_html) { SetBoolean("html", a_html); }

		inline std::string HTMLText() const { return GetString("htmlText"); }
		inline void HTMLText(std::string_view a_htmlText) { SetString("htmlText", a_htmlText); }

		inline double Length() const { return GetNumber("length"); }

		inline double MaxChars() const { return GetNumber("maxChars"); }
		inline void MaxChars(double a_maxChars) { SetNumber("maxChars", a_maxChars); }

		inline double MaxHScroll() const { return GetNumber("maxhscroll"); }

		inline double MaxScroll() const { return GetNumber("maxscroll"); }

		//ContextMenu Menu() const;

		inline bool MouseWheelEnabled() const { return GetBoolean("mouseWheelEnabled"); }
		inline void MouseWheelEnabled(bool a_mouseWheelEnabled) { SetBoolean("mouseWheelEnabled", a_mouseWheelEnabled); }

		inline bool Multiline() const { return GetBoolean("multiline"); }
		inline void Multiline(bool a_multiline) { SetBoolean("multiline", a_multiline); }

		inline std::string Name() const { return GetString("_name"); }
		inline void Name(std::string_view a_name) { SetString("_name", a_name); }

		//MovieClip Parent() const;
		//void Parent(const MovieClip& a_parent);

		inline bool Password() const { return GetBoolean("password"); }
		inline void Password(bool a_password) { SetBoolean("password", a_password); }

		inline std::string Quality() const { return GetString("_quality"); }
		inline void Quality(std::string_view a_quality) { SetString("_quality", a_quality); }

		inline std::string Restrict() const { return GetString("restrict"); }
		inline void Restrict(std::string_view a_restrict) { SetString("restrict", a_restrict); }

		inline double Rotation() const { return GetNumber("_rotation"); }
		inline void Rotation(double a_rotation) { SetNumber("_rotation", a_rotation); }

		inline double Scroll() const { return GetNumber("scroll"); }
		inline void Scroll(double a_scroll) { SetNumber("scroll", a_scroll); }

		inline bool Selectable() const { return GetBoolean("selectable"); }
		inline void Selectable(bool a_selectable) { SetBoolean("selectable", a_selectable); }

		inline double Sharpness() const { return GetNumber("sharpness"); }
		inline void Sharpness(double a_sharpness) { SetNumber("sharpness", a_sharpness); }

		inline double SoundBufTime() const { return GetNumber("_soundbuftime"); }
		inline void SoundBufTime(double a_soundBufTime) { SetNumber("_soundbuftime", a_soundBufTime); }

		//StyleSheet StyleSheet() const;
		//void StyleSheet(const StyleSheet& a_styleSheet);

		inline bool TabEnabled() const { return GetBoolean("tabEnabled"); }
		inline void TabEnabled(bool a_tabEnabled) { SetBoolean("tabEnabled", a_tabEnabled); }

		inline double TabIndex() const { return GetNumber("tabIndex"); }
		inline void TabIndex(double a_tabIndex) { SetNumber("tabIndex", a_tabIndex); }

		inline std::string Target() const { return GetString("_target"); }

		inline std::string Text() const { return GetString("text"); }
		inline void Text(std::string_view a_text) { SetString("text", a_text); }

		inline double TextColor() const { return GetNumber("textColor"); }
		inline void TextColor(double a_textColor) { SetNumber("textColor", a_textColor); }

		inline double TextHeight() const { return GetNumber("textHeight"); }
		inline void TextHeight(double a_textHeight) { SetNumber("textHeight", a_textHeight); }

		inline double TextWidth() const { return GetNumber("textWidth"); }
		inline void TextWidth(double a_textWidth) { SetNumber("textWidth", a_textWidth); }

		inline double Thickness() const { return GetNumber("thickness"); }
		inline void Thickness(double a_thickness) { SetNumber("thickness", a_thickness); }

		inline std::string Type() const { return GetString("type"); }
		inline void Type(std::string_view a_type) { SetString("type", a_type); }

		inline std::string URL() const { return GetString("_url"); }

		inline std::string Variable() const { return GetString("variable"); }
		inline void Variable(std::string_view a_variable) { SetString("variable", a_variable); }

		inline bool Visible() const { return GetBoolean("_visible"); }
		inline void Visible(bool a_visible) { SetBoolean("_visible", a_visible); }

		inline double Width() const { return GetNumber("_width"); }
		inline void Width(double a_width) { SetNumber("_width", a_width); }

		inline bool WordWrap() const { return GetBoolean("wordWrap"); }
		inline void WordWrap(bool a_wordWrap) { SetBoolean("wordWrap", a_wordWrap); }

		inline double X() const { return GetNumber("_x"); }
		inline void X(double a_x) { SetNumber("_x", a_x); }

		inline double XMouse() const { return GetNumber("_xmouse"); }

		inline double XScale() const { return GetNumber("_xscale"); }
		inline void XScale(double a_xScale) { SetNumber("_xscale", a_xScale); }

		inline double Y() const { return GetNumber("_y"); }
		inline void Y(double a_y) { SetNumber("_y", a_y); }

		inline double YMouse() const { return GetNumber("_ymouse"); }

		inline double YScale() const { return GetNumber("_yscale"); }
		inline void YScale(double a_yScale) { SetNumber("_yscale", a_yScale); }

		// methods
		inline bool AddListener(Object& a_listener)
		{
			enum
			{
				kListener,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kListener] = a_listener.GetInstance();
			assert(args[kListener].IsObject());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("addListener", std::addressof(boolean), args.data(), kNumArgs);
			assert(success);

			return boolean.GetBool();
		}

		inline double GetDepth()
		{
			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getDepth", std::addressof(number));
			assert(success);

			return number.GetNumber();
		}

		//TextFormat GetNewTextFormat();
		//TextFormat GetTextFormat(std::optional<double> a_beginIndex, std::optional<double> a_endIndex);

		inline bool RemoveListener(Object& a_listener)
		{
			enum
			{
				kListener,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kListener] = a_listener.GetInstance();
			assert(args[kListener].IsObject());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("removeListener", std::addressof(boolean), args.data(), args.size());
			assert(success);

			return boolean.GetBool();
		}

		inline void RemoveTextField()
		{
			[[maybe_unused]] const auto success =
				Invoke("removeTextField");
			assert(success);
		}

		inline void ReplaceSel(std::string_view a_newText)
		{
			enum
			{
				kNewText,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kNewText] = a_newText;
			assert(args[kNewText].IsString());

			[[maybe_unused]] const auto success =
				Invoke("replaceSel", nullptr, args.data(), args.size());
			assert(success);
		}

		inline void ReplaceText(double a_beginIndex, double a_endIndex, std::string_view a_newText)
		{
			enum
			{
				kBeginIndex,
				kEndIndex,
				kNewText,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kBeginIndex] = a_beginIndex;
			assert(args[kBeginIndex].IsNumber());

			args[kEndIndex] = a_endIndex;
			assert(args[kEndIndex].IsNumber());

			args[kNewText] = a_newText;
			assert(args[kNewText].IsString());

			[[maybe_unused]] const auto success =
				Invoke("replaceText", nullptr, args.data(), args.size());
			assert(success);
		}

		//void SetNewTextFormat(TextFormat& a_tf);

		//void SetTextFormat(std::optional<double> a_beginIndex, std::optional<double> a_endIndex, TextFormat& a_textFormat);;

		// gfx properties
		inline bool AutoFit() const { return GetBoolean("autoFit"); }
		inline void AutoFit(bool a_autoFit) { SetBoolean("autoFit", a_autoFit); }

		inline double CaretIndex() const { return GetNumber("caretIndex"); }
		inline void CaretIndex(double a_caretIndex) { SetNumber("caretIndex", a_caretIndex); }

		inline double FocusGroup() const { return GetNumber("focusGroup"); }
		inline void FocusGroup(double a_focusGroup) { SetNumber("focusGroup", a_focusGroup); }

		inline bool HitTestDisable() const { return GetBoolean("hitTestDisable"); }
		inline void HitTestDisable(bool a_hitTestDisable) { SetBoolean("hitTestDisable", a_hitTestDisable); }

		inline bool NoTranslate() const { return GetBoolean("noTranslate"); }
		inline void NoTranslate(bool a_noTranslate) { SetBoolean("noTranslate", a_noTranslate); }

		inline double NumLines() const { return GetNumber("numLines"); }
		inline void NumLines(double a_numLines) { SetNumber("numLines", a_numLines); }

		inline bool TopmostLevel() const { return GetBoolean("topmostLevel"); }
		inline void TopmostLevel(bool a_topmostLevel) { SetBoolean("topmostLevel", a_topmostLevel); }

		inline double InactiveSelectionBkgColor() const { return GetNumber("inactiveSelectionBkgColor"); }
		inline void InactiveSelectionBkgColor(double a_inactiveSelectionBkgColor) { SetNumber("inactiveSelectionBkgColor", a_inactiveSelectionBkgColor); }

		inline bool AlwaysShowSelection() const { return GetBoolean("alwaysShowSelection"); }
		inline void AlwaysShowSelection(bool a_alwaysShowSelection) { SetBoolean("alwaysShowSelection", a_alwaysShowSelection); }

		inline bool NoAutoSelection() const { return GetBoolean("noAutoSelection"); }
		inline void NoAutoSelection(bool a_noAutoSelection) { SetBoolean("noAutoSelection", a_noAutoSelection); }

		inline double SelectionBeginIndex() const { return GetNumber("selectionBeginIndex"); }
		inline void SelectionBeginIndex(double a_selectionBeginIndex) { SetNumber("selectionBeginIndex", a_selectionBeginIndex); }

		inline double SelectionEndIndex() const { return GetNumber("selectionEndIndex"); }
		inline void SelectionEndIndex(double a_selectionEndIndex) { SetNumber("selectionEndIndex", a_selectionEndIndex); }

		inline double SelectionBkgColor() const { return GetNumber("selectionBkgColor"); }
		inline void SelectionBkgColor(double a_selectionBkgColor) { SetNumber("selectionBkgColor", a_selectionBkgColor); }

		inline double SelectionTextColor() const { return GetNumber("selectionTextColor"); }
		inline void SelectionTextColor(double a_selectionTextColor) { SetNumber("selectionTextColor", a_selectionTextColor); }

		inline bool UseRichTextClipboard() const { return GetBoolean("useRichTextClipboard"); }
		inline void UseRichTextClipboard(bool a_useRichTextClipboard) { SetBoolean("useRichTextClipboard", a_useRichTextClipboard); }

		inline double InactiveSelectionTextColor() const { return GetNumber("inactiveSelectionTextColor"); }
		inline void InactiveSelectionTextColor(double a_inactiveSelectionTextColor) { SetNumber("inactiveSelectionTextColor", a_inactiveSelectionTextColor); }

		inline double FontScaleFactor() const { return GetNumber("fontScaleFactor"); }
		inline void FontScaleFactor(double a_fontScaleFactor) { SetNumber("fontScaleFactor", a_fontScaleFactor); }

		inline std::string TextAutoSize() const { return GetString("textAutoSize"); }
		inline void TextAutoSize(std::string_view a_textAutoSize) { SetString("textAutoSize", a_textAutoSize); }

		inline std::string VerticalAlign() const { return GetString("verticalAlign"); }
		inline void VerticalAlign(std::string_view a_verticalAlign) { SetString("verticalAlign", a_verticalAlign); }

		inline std::string VerticalAutoSize() const { return GetString("verticalAutoSize"); }
		inline void VerticalAutoSize(std::string_view a_verticalAutoSize) { SetString("verticalAutoSize", a_verticalAutoSize); }

		// gfx methods
		inline void AppendText(std::string_view a_newText)
		{
			enum
			{
				kNewText,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kNewText] = a_newText;
			assert(args[kNewText].IsString());

			[[maybe_unused]] const auto success =
				Invoke("appendText", nullptr, args.data(), args.size());
			assert(success);
		}

		inline void AppendHtml(std::string_view a_newHtml)
		{
			enum
			{
				kNewHtml,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kNewHtml] = a_newHtml;
			assert(args[kNewHtml].IsString());

			[[maybe_unused]] const auto success =
				Invoke("appendHtml", nullptr, args.data(), args.size());
			assert(success);
		}

		//Rectangle GetCharBoundaries(double a_charIndex);
		//Rectangle GetExactCharBoundaries(double a_charIndex);

		inline double GetCharIndexAtPoint(double a_x, double a_y)
		{
			enum
			{
				kX,
				kY,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kX] = a_x;
			assert(args[kX].IsNumber());

			args[kY] = a_y;
			assert(args[kY].IsNumber());

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getCharIndexAtPoint", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline double GetFirstCharInParagraph(double a_charIndex)
		{
			enum
			{
				kCharIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kCharIndex] = a_charIndex;
			assert(args[kCharIndex].IsNumber());

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getFirstCharInParagraph", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline double GetLineIndexAtPoint(double a_x, double a_y)
		{
			enum
			{
				kX,
				kY,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kX] = a_x;
			assert(args[kX].IsNumber());

			args[kY] = a_y;
			assert(args[kY].IsNumber());

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getLineIndexAtPoint", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline double GetLineLength(double a_lineIndex)
		{
			enum
			{
				kLineIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kLineIndex] = a_lineIndex;
			assert(args[kLineIndex].IsNumber());

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getLineLength", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline Object GetLineMetrics(double a_lineIndex)
		{
			enum
			{
				kLineIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kLineIndex] = a_lineIndex;
			assert(args[kLineIndex].IsNumber());

			RE::GFxValue object;
			[[maybe_unused]] const auto success =
				Invoke("getLineMetrics", std::addressof(object), args.data(), args.size());
			assert(success);

			return Object(object);
		}

		inline double GetLineOffset(double a_lineIndex)
		{
			enum
			{
				kLineIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kLineIndex] = a_lineIndex;
			assert(args[kLineIndex].IsNumber());

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("getLineOffset", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline std::string GetLineText(double a_lineIndex)
		{
			enum
			{
				kLineIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kLineIndex] = a_lineIndex;
			assert(args[kLineIndex].IsNumber());

			RE::GFxValue str;
			[[maybe_unused]] const auto success =
				Invoke("getLineText", std::addressof(str), args.data(), args.size());
			assert(success);

			return str.GetString();
		}

		inline void CopyToClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex)
		{
			enum
			{
				kRichClipboard,
				kStartIndex,
				kEndIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kRichClipboard] = a_richClipboard;
			assert(args[kRichClipboard].IsBool());

			args[kStartIndex] = a_startIndex;
			assert(args[kStartIndex].IsNumber());

			args[kEndIndex] = a_endIndex;
			assert(args[kEndIndex].IsNumber());

			[[maybe_unused]] const auto success =
				Invoke("copyToClipboard", nullptr, args.data(), args.size());
			assert(success);
		}

		inline void CutToClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex)
		{
			enum
			{
				kRichClipboard,
				kStartIndex,
				kEndIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kRichClipboard] = a_richClipboard;
			assert(args[kRichClipboard].IsBool());

			args[kStartIndex] = a_startIndex;
			assert(args[kStartIndex].IsNumber());

			args[kEndIndex] = a_endIndex;
			assert(args[kEndIndex].IsNumber());

			[[maybe_unused]] const auto success =
				Invoke("cutToClipboard", nullptr, args.data(), args.size());
			assert(success);
		}

		inline void PasteFromClipboard(bool a_richClipboard, double a_startIndex, double a_endIndex)
		{
			enum
			{
				kRichClipboard,
				kStartIndex,
				kEndIndex,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kRichClipboard] = a_richClipboard;
			assert(args[kRichClipboard].IsBool());

			args[kStartIndex] = a_startIndex;
			assert(args[kStartIndex].IsNumber());

			args[kEndIndex] = a_endIndex;
			assert(args[kEndIndex].IsNumber());

			[[maybe_unused]] const auto success =
				Invoke("pasteFromClipboard", nullptr, args.data(), args.size());
			assert(success);
		}
	};
}
