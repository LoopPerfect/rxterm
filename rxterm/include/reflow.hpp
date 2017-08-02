#ifndef RXTERM_REFLOW_HPP
#define RXTERM_REFLOW_HPP

namespace rxterm {

std::string reflow(unsigned const& width, std::string const& s) {

  if (width == 0) return "";
  if (s.size() <= width) { return s; }

  std::string content = "";
  auto b = 0;
  while (b < s.size()) {
    auto t = s.substr(b, width+1)
      .find_last_of("\n\t.,!?:;- ");

    if (t == std::string::npos) {
      content += s.substr(b, width);
      b += width;
      if (b < s.size()) {
        content += '\n';
      }
    } else {
      if (s[t] == '\n') {
        content += s.substr(b, t);
        b = t+1;
      } else if (t >= width) {
        content += s.substr(b, width) + '\n';
        b += width;
      } else {
        content += s.substr(b, t+1) + '\n';
        b += t+1;
      }
    }
  }

  return content;
}

}

#endif
