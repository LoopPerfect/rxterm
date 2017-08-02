#ifndef RXTERM_COMPONENTS_PROGRESS_HPP
#define RXTERM_COMPOMEMTS_PROGRESS_HPP

#include <rxterm/style.hpp>
#include <rxterm/utils.hpp>

namespace rxterm {

struct Progress {
  float progress;

  Progress(float p) : progress{p} {};

  std::string render(unsigned width)const {
    return std::string("\e[31m") + repeat(width*progress, "#") + "\e[49m\n";
  }
};

}

#endif
