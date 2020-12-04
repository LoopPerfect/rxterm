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
#endif

#include <rxterm/utils.hpp>

namespace rxterm {

struct VirtualTerminal {
  std::string buffer;

  std::string computeTransition(std::string const& next) const {
    if(buffer == next) return "";
    unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
    return clearLines(n) + "\033[0m" + next;
  }

  static std::string hide() { return "\033[0;8m"; }

  VirtualTerminal flip(std::string const& next) const {
    auto const transition = computeTransition(next);
    if(transition == "") return *this;
    std::cout << transition << hide();
    std::flush(std::cout);
    return {next};
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
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode)) return GetLastError();
  return ERROR_SUCCESS;
}

  VirtualTerminal() {
    std::call_once(initInstanceFlag, VirtualTerminal::initWindowsTerminal);
  }
  VirtualTerminal(const VirtualTerminal&) = default;
  VirtualTerminal& operator=(const VirtualTerminal&) = default;
  VirtualTerminal(const std::string &buf) : buffer{buf} {};
#endif
};

#ifdef _WIN32
std::once_flag VirtualTerminal::initInstanceFlag;
#endif
}

#endif
