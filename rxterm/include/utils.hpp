#ifndef RXTERM_UTILS_HPP
#define RXTERM_UTILS_HPP

#include <string>

namespace rxterm {

std::string repeat(unsigned n, std::string const& s) {
  std::string result = "";
  for(unsigned i=0; i<n;++i) {
    result+=s;
  }
  return result;
}

std::string clearAfterCursor() {
  return "\e[1K";
}

std::string clearLine() {
  return "\e[2K\r";
}


std::string moveUp(unsigned n=1) {
  return "\e["+std::to_string(n)+"A\r";
}


std::string clearLines(unsigned n=1) {
  return clearLine() + ((n)?repeat(n, moveUp()+clearLine()): std::string(""));
}

}

#endif
