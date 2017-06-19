#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <rxterm/terminal.hpp>


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

  vt = renderToTerm(vt, Content{"Hello\nWorld"});
  std::this_thread::sleep_for(1s);
  vt = renderToTerm(vt, Content{"and\nGood\nBye\nNew World!"});

  return 0;
}

