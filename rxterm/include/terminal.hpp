#ifndef RXTERM_TERMINAL_HPP
#define RXTERM_TERMINAL_HPP


#include <iostream>
#include <algorithm>
#include <string>
#include <rxterm/utils.hpp>

namespace rxterm {

struct VirtualTerminal {
  std::string buffer;

  std::string computeTransition(std::string const& next) const {
    if(buffer == next) return "";
    unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
    return clearLines(n) + "\e[0m;" + next;
  }

  static std::string hide() { return "\e[0;8m"; }

  VirtualTerminal flip(std::string const& next) const {
    auto const& transition = computeTransition(next);
    if(transition == "") return *this;
    std::cout << transition << hide();
    std::flush(std::cout);
    return {next};
  }
};

}

#endif
