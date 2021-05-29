# RxTerm ⚡⌨️

A C++ library for functional-reactive terminals. RxTerm is a lean alternative to ncurses for many applications.

![terminal](https://cdn-images-1.medium.com/max/800/1*G897-DuUI_0q6W9VKcIjZg.gif)


## Build Instructions

The library builds with [Buckaroo](https://buckaroo.pm) and either [Buck](https://www.buckbuild.com) or [Bazel](https://bazel.build). It requires a C++ 14 compiler.

```bash
buckaroo install

# Buck
buck build :rxterm

# Bazel
bazel build :rxterm

# CMake
mkdir build
cd build
cmake ..
make
```

To run the demo:

```bash
# Buck
buck run :main

# Bazel
BAZEL_CXXOPTS="-std=c++14" bazel run :main

# CMake (inside the build dir)
cd apps/
./rxterm-demo
```


## Usage

To install this library into your project:

```bash=
buckaroo add github.com/loopperfect/rxterm@*
```

Refer to `rxterm/apps/main.cpp` for an example.


## Progress

 * [x] ANSI Output
 * [x] Reusable components
 * [ ] Managed console input
 * [ ] Terminal width detection
 * [ ] Windows support

Contributions are very welcome! 💖

## Windows Support

- VT100 escape sequences are enabled by default in msys2, mingw and cygwin.
- Cmd.exe and Powershell may require explicit activation of terminal mode. The easiest way is to call system(" ") at the start of your application.

More info:
https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
https://larsfosdal.blog/2018/08/31/did-you-know-that-you-can-enable-vt100-terminal-emulation-in-powershell-as-well-as-the-cmd-window/
