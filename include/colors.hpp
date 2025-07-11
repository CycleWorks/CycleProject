#pragma once

#include "utils.hpp"
#include <unordered_map>

namespace Cycle {
    // Console text color (Values may differ between different systems)
    enum class OutputColor;

    #ifdef OS_SUPPORTS_ANSI_ESCAPES // POSIX-compatible
        #define USE_COLOR_CONSOLE

        enum class OutputColor {
            RESET = "\033[0m",
            BLACK = "\033[30m",
            BLUE = "\033[34m",
            GREEN = "\033[32m",
            CYAN = "\033[36m",
            RED = "\033[31m",
            MAGENTA = "\033[35m",
            YELLOW = "\033[33m",
            WHITE = "\033[37m",
            GREY = "\033[37m"
        };
    #elifdef OS_WINDOWS // If OS is trash
        #define USE_COLOR_CONSOLE

        enum class OutputColor {
            RESET = -1,
            BLACK = 0,
            BLUE = FOREGROUND_BLUE,
            GREEN = FOREGROUND_GREEN,
            CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
            RED = FOREGROUND_RED,
            MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
            YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
            WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            GREY = 8
        };
    #else
        #warning Compiling on operating system with no supported console color support
    #endif

    void set_console_color(const OutputColor& color) noexcept;

    using ColorName = std::string;

    inline const ColorName RESET_COLOR = "@rs";
    inline const ColorName BLACK_COLOR = "@bk";
    inline const ColorName BLUE_COLOR = "@bl";
    inline const ColorName GREEN_COLOR = "@gn";
    inline const ColorName CYAN_COLOR = "@cy";
    inline const ColorName RED_COLOR = "@rd";
    inline const ColorName MAGENTA_COLOR = "@mg";
    inline const ColorName YELLOW_COLOR = "@yw";
    inline const ColorName WHITE_COLOR = "@wh";
    inline const ColorName GREY_COLOR = "@gy";

    inline const std::unordered_map<ColorName, OutputColor> COLOR_CODES = {
        {RESET_COLOR, OutputColor::RESET},
        {BLACK_COLOR, OutputColor::BLACK},
        {BLUE_COLOR, OutputColor::BLUE},
        {GREEN_COLOR, OutputColor::GREEN},
        {CYAN_COLOR, OutputColor::CYAN},
        {RED_COLOR, OutputColor::RED},
        {MAGENTA_COLOR, OutputColor::MAGENTA},
        {YELLOW_COLOR, OutputColor::YELLOW},
        {WHITE_COLOR, OutputColor::WHITE},
        {GREY_COLOR, OutputColor::GREY}
    };
}
