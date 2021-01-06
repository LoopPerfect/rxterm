#ifndef RXTERM_IMAGE_HPP
#define RXTERM_IMAGE_HPP

#include <string>
#include <vector>

#include <rxterm/pixel.hpp>
#include <rxterm/style.hpp>
#include <rxterm/utils.hpp>

namespace rxterm {

struct Image {

  std::vector<Pixel> pixels;
  unsigned width;
  unsigned height;

  Image(
    std::vector<Pixel> const& pixels,
    unsigned const width,
    unsigned const height)
    : pixels{pixels}
    , width{width}
    , height{height}
  {}

  Image() = default;
  Image(Image const&) = default;
  Image& operator=(Image const&) = default;


  static Image create(unsigned const& width, unsigned const& height, Pixel const& pixel = Pixel{'\0', Style::None()}) {
    return {
      std::vector<Pixel>(width*height, pixel),
      width,
      height
    };
  }



  Pixel const& operator()(unsigned int x, unsigned int y)const {
    return pixels[y*width+x];
  }


  Pixel& operator()(unsigned int x, unsigned int y) {
    return pixels[y*width+x];
  }


  std::string toString()const {
    std::string str = "";

    auto prev = Style{};
    for (unsigned int y=0; y < height; ++y) {
      for (unsigned int x=0; x < width; ++x) {
        auto const& pixel = (*this)(x, y);
        auto const current = diff(prev, pixel.c ? pixel.style : Style::Default());
        char c = pixel.c ? pixel.c : ' ';
        str += current.toString() + c;
        prev = pixel.c != '\0' ? pixel.style : Style::Default();
      }
      str+="\n";
    }
    return str;
  }
};

struct Sprite {
  Image image;
  int x = 0;
  int y = 0;
};

Image drawOnBackground(Image canvas, unsigned const& sx, unsigned const& sy, Image const& fg) {
  for (unsigned int y=0; y < fg.height; ++y) {
    for (unsigned int x=0; x < fg.width; ++x) {
      auto& p = canvas(
        clip(sx+x, 0u, canvas.width), 
        clip(sy+y, 0u, canvas.height)
      );
      auto const& q = fg(x, y);
      p = Pixel{
        (q.c)? q.c : p.c,
        Style{
          q.style.fg,
          (q.c)? q.style.bg : p.style.bg,
          q.style.font
        }
      };
    }
  }
  return canvas;
}

Image drawOnBackground(Image canvas, Sprite const& s) {
  return drawOnBackground(canvas, s.x, s.y, s.image);
}

Image drawOnBackground(Image const& canvas) {
  return canvas;
}

template<class X, class...Xs>
auto drawOnBackground(Image const& canvas, X const& s, Xs const&...xs) 
  -> decltype( s.image, s.x, s.y , drawOnBackground(canvas, xs...)) {
  return drawOnBackground(
    drawOnBackground(canvas, s),
    xs...
  );
}

Image drawOnBackground(Image canvas, std::vector<Sprite> const& sprites) {
  for (auto const& s: sprites) {
    canvas = drawOnBackground(canvas, s);
  }

  return canvas;
}

}


#endif
