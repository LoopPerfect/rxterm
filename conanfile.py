from conans import ConanFile, CMake


class RxtermConan(ConanFile):
    name = "simplendian"
    version = "0.0.1"
    license = "MIT"
    author = "LoopPerfect hello@loopperfect.com"
    url = "https://github.com/kindlychung/rxterm"
    description = "A C++ library for functional-reactive terminals. RxTerm is a lean alternative to ncurses for many applications."
    topics = ("terminal", "cpp", "tui", "ui")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports_sources = "rxterm/include/*"
    no_copy_source = True

    def package(self):
        self.copy("*", dst="include/rxterm", src="rxterm/include")
