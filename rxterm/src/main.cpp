#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <rxterm/terminal.hpp>

std::string reflow(unsigned width, std::string const& s) {

  std::string content = "";
  auto b = 0;

  if (width == 0) return "";

  if (s.size() <= width) { return s; }

  while (b < s.size()) {
    auto t = s.substr(b, width+1)
      .find_last_of("\n\t.,!?:;- ");

    if (t == std::string::npos) {
      content += s.substr(b, width);
      b+=width;
      if( b < s.size()) {
        content +='\n';
       }
    } else {
      if (s[t] == '\n') {
        content += s.substr(b, t);
        b=t+1;
      } else if (t >= width) {
        content += s.substr(b, width)+'\n';
        b+=width;
      } else {
        content += s.substr(b, t+1)+'\n';
        b=b+t+1;
      }
    }
  }



  return content;
}

struct Content {
  std::string content;
  std::string render(unsigned width)const {
    return content;
  }
};


auto renderToTerm = [](auto vt, auto const& c) {
  //TODO: get actual terminal width
  vt.flip(c.render(80));
  return vt;
};

int main() {
  using namespace std::chrono_literals;
  using namespace rxterm;

  std::cout << std::endl;
  VirtualTerminal vt;

  auto s = reflow(4, "12345-abcde-12345-ab");



  std::cout << std::endl <<s << std::endl;



  return 0;
}

