#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/reflow.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>

using namespace rxterm;

struct Content {
  std::string content;

  std::string render(unsigned width)const {
    return reflow(width, content);
  }
};


auto renderToTerm = [](auto vt, auto const& c) {
  //TODO: get actual terminal width
  vt.flip(c.toString());
  return vt;
};

int main() {
  using namespace std::chrono_literals;
  using namespace rxterm;
  using namespace std::string_literals;

  VirtualTerminal vt;


  auto const c1 = FlowLayout<>({
    Text({
      FgColor::Red,
      BgColor::Blue},
      "to stri\nng"s)
  });


  auto const c2 = FlowLayout<>({
    Text({
     FgColor::Red,
     BgColor::Blue,
     Font::Crossed},
      "to stri\n-ng"s),

    Text({
      FgColor::Blue,
      BgColor::Green,
      Font::Underline},
      "first\n mult\ni"s),
  });


  auto const img1 = c1.render(100);
  auto const img2 = c2.render(100);

  vt = renderToTerm(vt, img1);
  std::this_thread::sleep_for(1s);
  vt = renderToTerm(vt, img2);
  std::this_thread::sleep_for(1s);
  vt = renderToTerm(vt, img1);

  return 0;
}

