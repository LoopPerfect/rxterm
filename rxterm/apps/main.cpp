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

auto renderToTerm = [](auto vt, unsigned w, Component const& c) {
  //TODO: get actual terminal width
  vt.flip(c.render(w).toString());
  return vt;
};

int main() {
  using namespace std::chrono_literals;
  using namespace rxterm;
  using namespace std::string_literals;

  VirtualTerminal vt;

  auto superProgressBar = [](auto x, auto y, auto z) -> FlowLayout<> {
    return {
      Text("3 segment progressbar: "),
      FlowLayout<>{
        MaxWidth(20, Progress(x)),
        MaxWidth(20, Progress(y)),
        MaxWidth(20, Progress(z))
      }
    };
  };

  for(int i=0; i<101; ++i) {
    vt = renderToTerm(vt, 80, superProgressBar(0.01*i, 0.02*i, 0.03*i));
    std::this_thread::sleep_for(200ms);
  }

  return 0;
}

