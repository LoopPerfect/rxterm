#ifndef RXTERM_COMPONENTS_FLOWLAYOUT_HPP
#define RXTERM_COMPOMEMTS_FLOWLAYOUT_HPP

#include <rxterm/image.hpp>
#include <rxterm/components/component.hpp>
#include <rxterm/utils.hpp>
#include <rxterm/style.hpp>
#include <rxterm/reflow.hpp>
#include <algorithm>
#include <iostream>

namespace rxterm {

template<class T = Component>
struct FlowLayout {
  std::vector<T> children;
  Pixel bg;

  FlowLayout(std::vector<T> const& children, Pixel bg = Pixel{})
    : children(children)
    , bg{bg}
  {}


  Image render(int maxWidth) const {

    auto const images = map(children, [maxWidth](auto const& c) {
      auto const image = c.render(maxWidth);
      return image;
    });

    unsigned width = 0;
    unsigned curWidth = 0;
    unsigned curHeight = 0;
    std::vector<unsigned> ys;
    unsigned baseline = 0;

    for (auto const& image : images) {
      curHeight = std::max(curHeight, image.height);
      curWidth += image.width;

      ys.push_back(baseline);
      if (curWidth > maxWidth) {
        curWidth = 0;
        baseline += curHeight;
        curHeight = 0;
      }

      width = std::max(width, curWidth);
    }

    auto const height = baseline + curHeight;
    auto canvas = Image::create(width, height, bg);

    auto x = 0;
    auto yi = ys.cbegin();
    for(auto const& image : images) {
      drawOnBackground(canvas, x, *yi, image);
      x += image.width;
      ++yi;
    }

    return canvas;
  }
};

}

#endif
