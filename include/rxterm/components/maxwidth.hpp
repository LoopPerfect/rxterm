#ifndef RXTERM_COMPONENTS_MAXWIDTH_HPP
#define RXTERM_COMPONENTS_MAXWIDTH_HPP

#include <rxterm/image.hpp>
#include <rxterm/components/component.hpp>
#include <cmath>
#include <functional>

namespace rxterm {

auto percent(float const& p) {
  return [=](auto const& width) -> unsigned {
    return p/100 * width;
  };
}

auto px(unsigned const& width) {
  return [=](auto) -> unsigned {
    return width;
  };
}

struct MaxWidth {
  std::function<unsigned(unsigned)> maxWidth;
  Component const c;

  MaxWidth(std::function<unsigned(unsigned)> maxWidth, Component const& c)
    : maxWidth{maxWidth}
    , c{c}
  {}

  MaxWidth(unsigned const& maxWidth, Component const& c)
    : maxWidth{px(maxWidth)}
    , c{c}
  {}

  Image render(unsigned const width)const {
    return c.render(std::min(width, maxWidth(width)));
  }
};

}

#endif
