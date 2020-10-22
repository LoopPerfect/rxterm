#ifndef RXTERM_PIXEL_HPP
#define RXTERM_PIXEL_HPP

#include <string>
#include <rxterm/style.hpp>

namespace rxterm {

struct Pixel {
  char c ='\0';
  Style style;

  std::string toString() const {
    return style.toString() + c;
  }
};

}

#endif
