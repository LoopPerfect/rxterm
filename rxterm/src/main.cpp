#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>

std::string reflow(unsigned width, std::string const& s) {

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

struct Content {
  std::string content;

  std::string render(unsigned width)const {
    return reflow(width, content);
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

   auto s1 = Style(Color::White, Color::Green, Font::Crossed);
   auto s2 = Style(
       Color::Blue,
       Color::White,
       fonts(
         Font::Underline,
         Font::Crossed,
         Font::Italic
       ));

   auto s3 = Style(
       Color::Red,
       Color::Blue,
       fonts(
         Font::Bold,
        // Font::Crossed,
         Font::Italic
       ));


  std::cout << s1.render() <<"lol"<< "\e[0m"<<std::endl;
  std::cout << s2.render() <<"lol"<< "\e[0m"<<std::endl;
  std::cout << s1.render() << diff(s1, s3).render().substr(1) << std::endl;
  return 0;
}

