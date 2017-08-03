#ifndef RXTERM_COMPONENTS_FLOWLAYOUT_HPP
#define RXTERM_COMPOMEMTS_FLOWLAYOUT_HPP

#include <rxterm/image.hpp>
#include <rxterm/components/component.hpp>
#include <cmath>

namespace rxterm {

struct MaxWidth {
  unsigned const maxWidth;
  Component const c;

  MaxWidth(unsigned const maxWidth, Component const& c)
    : maxWidth{maxWidth}
    , c{c}
  {}

  Image render(unsigned width)const {
    return c.render(std::min(width, maxWidth));
  }
};

}

#endif
