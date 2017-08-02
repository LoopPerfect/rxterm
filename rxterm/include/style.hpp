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

enum class Color {
  Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7,
  Default = 9,
  Transparent = 10,
  Inherit = 11
};



template<class X>
constexpr std::string composeMod(X x) {
  return x;
}

template<class X, class...Xs>
constexpr std::string composeMod(X x, Xs...xs) {
  auto const r = composeMod(xs...);
  auto const delim = (r == "") || (x == "") ? "" : ";";
  return x + delim + r;
}

template<class X, class...Xs>
constexpr std::string computeMod(X x, Xs...xs) {
  auto const  r = composeMod(x, xs...);
  return (r == "") ? "" : "\e["+r+"m";
}

constexpr Font fonts (){
  return Font::None;
}



template<class X>
constexpr Font fonts (X x){
  return x;
}


template<class X, class...Xs>
constexpr Font fonts(X x, Xs...xs) {
  return Font( ((int)x) | ((int)fonts(xs...))  );
}


bool has(Font x, Font y) {
  return (((int)x) & ((int)y));
}



struct Style {
  Color bg;
  Color fg;
  Font font;

  template<class...Fonts>
  constexpr Style(
      Color bg = Color::Inherit,
      Color fg = Color::Inherit,
      Fonts...fs)
    : bg{bg}
    , fg{fg}
    , font{fonts(fs...)}
  {}


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
    return ((int)bg<10) ? std::to_string(40 + (int)bg) : "";
  }

  string fgMod() const {
    return ((int)fg<10) ? std::to_string(30 + (int)fg) : "";
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


Style diff(Style const& a, Style const& b=Style{} ) {
  bool keepBG = (a.bg == b.bg);
  bool keepFG = (a.fg == b.fg);
  bool keepFont = (a.font == b.font);

  int l = (int)a.font;
  int r = (int)b.font;
  bool reset =  (l & ~r)? 1 : 0;

  return Style {
    (keepBG && !reset) ? Color::Inherit : b.bg,
    (keepFG && !reset) ? Color::Inherit : b.fg,
    (keepFont && !reset) ? Font::Inherit : (Font)((r&((l&r)^r))|reset)
  };
}


Style cascade(Style const& p, Style const& c=Style() ) {
  bool inheritBG = (c.bg == Color::Inherit);
  bool inheritFG = (c.fg == Color::Inherit);
  bool inheritFont = (c.font == Font::Inherit);

  return Style {
    inheritBG ? p.bg : c.bg,
    inheritFG ? p.fg : c.fg,
    inheritFont ? p.font : c.font,
  };
}


}

#endif
