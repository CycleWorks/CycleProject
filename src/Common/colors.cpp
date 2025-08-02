#include "Common/colors.hpp"
#include <print>

int _windows_default_color; // Set in _init_colors

void _init_colors(){
    #ifdef OS_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO info;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hConsole, &info)){
            _windows_default_color = info.wAttributes & 0x0F;
        }
    #endif
    set_console_color(Cycle::OutputColor::RESET);
    std::atexit([](){
        set_console_color(Cycle::OutputColor::RESET);
    });
}

void Cycle::set_console_color(const OutputColor& color) noexcept {
    static bool initialized = false;
    if (!initialized){
        initialized = true;
        _init_colors();
    }
    #ifdef OS_SUPPORTS_ANSI_ESCAPES
        std::print("{}", (const char*)foreground_color);
    #elifdef OS_WINDOWS
        if (color == OutputColor::RESET){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _windows_default_color);
            return;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (unsigned int)color);
    #else
        return; // Do nothing, colors not supported
    #endif
}
