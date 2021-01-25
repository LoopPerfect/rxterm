#ifndef RXTERM_TERMINAL_HPP
#define RXTERM_TERMINAL_HPP

#include <algorithm>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <csignal>
#include <mutex>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <rxterm/utils.hpp>
#include <utility>

namespace rxterm
{

  volatile std::sig_atomic_t& gSignalStatus()
  {
    static volatile std::sig_atomic_t instance = 0;
    return instance;
  }

  struct VirtualTerminal
  {
    std::string buffer;
    static unsigned width()
    {
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
    static unsigned height()
    {
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

    std::string computeTransition(std::string const& next) const
    {
      if (buffer == next)
        return "";
      unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
      return clearLines(n) + "\033[0m" + next;
    }

    static std::string hide()
    {
      return "\033[0;8m";
    }

    VirtualTerminal flip(std::string const& next) const
    {
      auto const transition = computeTransition(next);
      if (transition.empty())
        return *this;
      std::cout << transition << hide();
      std::flush(std::cout);
      return VirtualTerminal{ next };
    }

    struct InitSignalHandlerThread
    {
      static void globalInit()
      {
        static InitSignalHandlerThread _;
      }

    private:
      InitSignalHandlerThread()
      {

        std::signal(SIGINT, [](int signal) { gSignalStatus() = signal; });

        std::thread signal_handler_thread([] {
          using namespace std::chrono_literals;
          for (;;)
          {
            if (gSignalStatus() != 0)
            {
              
              std::exit(gSignalStatus());
            }
            std::this_thread::sleep_for(100ms);
          }
          });
        signal_handler_thread.detach();
      }
    };

    struct InitAlternateBuffer
    {
      static void globalInit()
      {
        static InitAlternateBuffer _;
      }

    private:
      InitAlternateBuffer()
      {
        // Enter the alternate buffer
        std::cout << "\x1b[?1049h";
      }
      ~InitAlternateBuffer()
      {
        // Exit the alternate buffer
        std::cout << "\x1b?1049l";
        // Returns all attributes to the default state prior to modification
        std::cout << "\033[0;m";
      }
    };

#ifdef _WIN32
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
    struct InitVirtualConsoleMode
    {
      static void globalInit()
      {
        static InitVirtualConsoleMode _;
      }

    private:
      static void setVirtualTerminalProcessingMode(bool value)
      {
        // Set output mode to handle virtual terminal sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
          throw std::system_error(
            std::error_code(GetLastError(), std::system_category()));
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
          throw std::system_error(
            std::error_code(GetLastError(), std::system_category()));
        if (value)
        {
          dwMode |= DWORD(ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        else
        {
          dwMode &= ~DWORD(ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        if (!SetConsoleMode(hOut, dwMode))
          throw std::system_error(
            std::error_code(GetLastError(), std::system_category()));
      }

      InitVirtualConsoleMode()
      {
        setVirtualTerminalProcessingMode(true);
      }
      ~InitVirtualConsoleMode()
      {
        setVirtualTerminalProcessingMode(false);
      }
    };
#endif

    VirtualTerminal(bool installInterruptSignalHandlerThread = true,
      bool useAlternateBuffer = true)
    {
#ifdef _WIN32
      InitVirtualConsoleMode::globalInit();
#endif

      if (useAlternateBuffer)
      {
        InitAlternateBuffer::globalInit();
      }
      if (installInterruptSignalHandlerThread)
      {
        InitSignalHandlerThread::globalInit();
      }
    }

    VirtualTerminal(const VirtualTerminal&) = default;
    VirtualTerminal& operator=(const VirtualTerminal&) = default;
    explicit VirtualTerminal(std::string buf) : buffer{ std::move(buf) } {};
  };

}

#endif

