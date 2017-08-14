#ifndef RXTERM_STYLE_HPP
#define RXTERM_STYLE_HPP

#include <string>

namespace rxterm {

using std::string;

enum class Font {
  None = 0,
  Default = 1,
  Bold = 1 << 1, // 1
  Faint = 1 << 2, // 2
  Roman = 1 << 3,// not Italic
  Italic = 1 << 4, // 3
  NotUnderline = 1 << 5,
  Underline = 1 << 6, //4
  Visible = 1 << 7,
  Hidden = 1 << 8, //8
  Uncrossed = 1 << 9,
  Crossed = 1 << 10, // 9
  Inherit = 1 << 11
};

enum class FontColor {
  None = 0,
  Black = 1,
  Red = 2,
  Green = 3,
  Yellow = 4,
  Blue = 5,
  Magenta = 6,
  Cyan = 7,
  White = 8,
  Default = 9,
  Transparent = 10,
  Inherit = 11
};


enum class Color {
  None = 0,
  Black = 1,
  Red = 2,
  Green = 3,
  Yellow = 4,
  Blue = 5,
  Magenta = 6,
  Cyan = 7,
  White = 8,
  Default = 9,
  Transparent = 10,
  Inherit = 11
};



template<class X>
constexpr auto composeMod(X x) {
  return x;
}

template<class X, class...Xs>
constexpr auto composeMod(X x, Xs...xs) {
  auto const r = composeMod(xs...);
  auto const delim = (r == "") || (x == "") ? "" : ";";
  return x + delim + r;
}

template<class X, class...Xs>
constexpr auto computeMod(X x, Xs...xs) {
  auto const  r = composeMod(x, xs...);
  return (r == "") ? "" : "\e[" + r + "m";
}

FontColor isStyle(FontColor x) { return x; }
Color isStyle(Color x) { return x; }
Font isStyle(Font x) { return x; }

FontColor toFontColor(FontColor c) { return c; }
Color toColor(Color c) { return c; }
Font toFont(Font f) { return f; }

FontColor toFontColor(...) { return FontColor::None; }
Color toColor(...) { return Color::None; }
Font toFont(...) { return Font::None; }

template<class...Xs>
constexpr FontColor getFontColor (Xs...xs) {
  return (FontColor)std::max({0, (int)toFontColor(xs)...});
}

template<class...Xs>
constexpr Color getColor (Xs...xs) {
  return (Color)std::max({0, (int)toColor(xs)...});
}

constexpr Font getFontStyle(Font f = Font::None) {
  return f;
}



template<class X, class...Xs>
constexpr Font getFontStyle(X x, Xs...xs) {
  return Font( ((int)getFontStyle(toFont(x))) | ((int)getFontStyle(toFont(xs)...))  );
}


bool has(Font x, Font y) {
  return (((int)x) & ((int)y));
}

struct Style {
  Color bg;
  FontColor fg;
  Font font;

  template<class...Styles>
  constexpr Style(
    Styles...styles)
    : bg{getColor(isStyle(styles)...)}
    , fg{getFontColor(isStyle(styles)...)}
    , font{getFontStyle(isStyle(styles)...)}
  {}

  constexpr static Style None(){ return{}; }
  constexpr static Style Default(){ return{Font::Default}; }

  string defaultMod() const {
    return has(font, Font::Default) ? "0" : "";
  }


  string boldMod() const {
    return has(font, Font::Bold) ? "1" : "";
  }

  string underlineMod() const {
    return has(font, Font::Underline) ? "4" : "";
  }

  string faintMod() const {
    return has(font, Font::Faint) ? "2" : "";
  }

  string italicMod() const {
    return has(font, Font::Italic) ? "3" : "";
  }

  string hiddenMod() const {
    return has(font, Font::Hidden) ? "8" : "";
  }

  string crossedMod() const {
    return has(font, Font::Crossed) ? "9" : "";
  }


  string bgMod() const {
    return ((int)bg<11) ? std::to_string(40 + (int)bg -1) : "";
  }

  string fgMod() const {
    return ((int)fg<11) ? std::to_string(30 + (int)fg -1) : "";
  }

  std::string toString() const {
    return computeMod(
        defaultMod(),
        boldMod(),
        underlineMod(),
        faintMod(),
        italicMod(),
        hiddenMod(),
        crossedMod(),
        bgMod(),
        fgMod()
    );
  }
};


Style diff(Style const& a, Style const& b = Style::None() ) {
  bool keepBG = (a.bg == b.bg);
  bool keepFG = (a.fg == b.fg);
  bool keepFont = (a.font == b.font);

  int l = (int)a.font;
  int r = (int)b.font;
  bool reset =  (l & ~r)? 1 : 0;

  return Style {
    (keepBG && !reset) ? Color::Inherit : b.bg,
    (keepFG && !reset) ? FontColor::Inherit : b.fg,
    (keepFont && !reset) ? Font::Inherit : (Font)((r&((l&r)^r))|reset)
  };
}

}

#endif
