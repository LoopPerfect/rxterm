# RxTerm ⚡⌨️

A C++ library for functional-reactive terminals. RxTerm is a lean alternative to ncurses for many applications.

![terminal](https://cdn-images-1.medium.com/max/800/1*G897-DuUI_0q6W9VKcIjZg.gif)


## Build Instructions

The library builds with [Buckaroo](https://buckaroo.pm) and [Buck](https://www.buckbuild.com). It requires a C++ 14 compiler.

```bash
buckaroo install
buck build :rxterm
```

To run the demo:

```bash
buck run :main
```


## Usage

To install this library into your project:

```bash=
buckaroo add github.com/loopperfect/rxterm
```

Refer to `rxterm/apps/main.cpp` for an example.


## Progress

 * [x] ANSI Output
 * [x] Reusable components
 * [ ] Managed console input
 * [ ] Terminal width detection
 * [ ] Windows support

Contributions are very welcome! 💖
