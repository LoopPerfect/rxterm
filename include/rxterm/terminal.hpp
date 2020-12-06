#ifndef RXTERM_TERMINAL_HPP
#define RXTERM_TERMINAL_HPP

#include <iostream>
#include <algorithm>
#include <string>

#ifdef _WIN32
#include <mutex>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <utility>
#include <rxterm/utils.hpp>

namespace rxterm {

struct VirtualTerminal {
  std::string buffer;
  static unsigned width() {
    unsigned w;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    w = (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
      ? info.srWindow.Right - info.srWindow.Left + 1
      : 0;
#else
    winsize ws{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    w = ws.ws_col;
#endif
    return w;
  }
  static unsigned height() {
    unsigned h;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    h = (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
      ? info.srWindow.Bottom - info.srWindow.Top + 1
      : 0;
#else
      winsize ws{};
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
      h = ws.ws_row;
#endif
    return h;
  }

  std::string computeTransition(std::string const& next) const {
    if(buffer == next) return "";
    unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
    return clearLines(n) + "\033[0m" + next;
  }

  static std::string hide() { return "\033[0;8m"; }

  VirtualTerminal flip(std::string const& next) const {
    auto const transition = computeTransition(next);
    if(transition.empty()) return *this;
    std::cout << transition << hide();
    std::flush(std::cout);
    return VirtualTerminal{next};
  }

#ifdef _WIN32
  static std::once_flag initInstanceFlag;
  static DWORD initWindowsTerminal() {
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return GetLastError();
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return GetLastError();
    dwMode |= DWORD(ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!SetConsoleMode(hOut, dwMode)) return GetLastError();
    return ERROR_SUCCESS;
  }

  VirtualTerminal() {
    std::call_once(initInstanceFlag, VirtualTerminal::initWindowsTerminal);
  }
  VirtualTerminal(const VirtualTerminal&) = default;
  VirtualTerminal& operator=(const VirtualTerminal&) = default;
  explicit VirtualTerminal(std::string buf) : buffer{std::move(buf)} {};
#endif
};

#ifdef _WIN32
std::once_flag VirtualTerminal::initInstanceFlag;
#endif
}

#endif
