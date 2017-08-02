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
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>
using namespace rxterm;

auto renderToTerm = [](auto vt, unsigned w, auto const& c) {
  //TODO: get actual terminal width
  vt.flip(c.render(w).toString());
  return vt;
};

int main() {
  using namespace std::chrono_literals;
  using namespace rxterm;
  using namespace std::string_literals;

  VirtualTerminal vt;

  Component superProgressBar = []{
    return FlowLayout<>{{
      MaxWidth(10, Progress(0.1)),
      MaxWidth(10, Progress(0.2)),
      MaxWidth(10, Progress(0.3))
    }};
  };


  vt = renderToTerm(vt, 30, superProgressBar);
  std::this_thread::sleep_for(1s);

  vt = renderToTerm(vt, 30, MaxWidth(30, Progress(0.1)));
  std::this_thread::sleep_for(1s);

  vt = renderToTerm(vt, 30, Progress(0.3));
  std::this_thread::sleep_for(1s);

  vt = renderToTerm(vt, 30, Progress(0.4));
  std::this_thread::sleep_for(1s);

  return 0;
}

