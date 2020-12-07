#include <thread>
#include <chrono>

#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>

using namespace rxterm;

auto renderToTerm = [](auto const& vt, unsigned const w, Component const& c) {
  return vt.flip(c.render(w).toString());
};

int main() {

  using namespace std::chrono_literals;
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

  auto w = VirtualTerminal::width();
  if (!w) w = 80;
  for (int i = 0; i < 101; ++i) {
    vt = renderToTerm(vt, w, superProgressBar(0.01 * i, 0.02 * i, 0.03 * i));
    std::this_thread::sleep_for(200ms);
  }

  return 0;
}
