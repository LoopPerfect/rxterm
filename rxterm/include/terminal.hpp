#ifndef RXTERM_TERMINAL_HPP
#define RXTERM_TERMINAL_HPP


#include <iostream>
#include <algorithm>
#include <string>
#include <rxterm/utils.hpp>

namespace rxterm {

struct VirtualTerminal {
  std::string buffer;

  void reset() {
    unsigned n = std::count(buffer.begin(), buffer.end(), '\n')+1;
    std::cout << clearLines(n) << std::endl;
    buffer = "";
  }

  void flip(std::string s) {
    reset();
    std::cout << s << std::endl;
    buffer = s;
  }
};

}

#endif
