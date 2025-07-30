#include "colors.hpp"

#ifdef OS_SUPPORTS_ANSI_ESCAPES
    void Cycle::set_console_color(const OutputColor& foreground_color) noexcept {
        std::print("{}", (const char*)foreground_color);
    }
#elifdef OS_WINDOWS
    void Cycle::set_console_color(const OutputColor& color) noexcept {
        if (color == OutputColor::RESET){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (unsigned int)OutputColor::WHITE);
            return;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (unsigned int)color);
    }
#else
    void Cycle::set_console_color(const OutputColor& foreground_color) noexcept {
        return; // Do nothing
    }
#endif

// Create and initialize colors
struct InitColors {
    InitColors(){
        set_console_color(Cycle::OutputColor::RESET);
        std::atexit([](){
            set_console_color(Cycle::OutputColor::RESET);
        });
    }
};
InitColors init;
