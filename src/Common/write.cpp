#include "write.hpp"
#include "colors.hpp"

#include <iostream>
#include <mutex>

void Cycle::basic_writeln(std::ostream& stream, const std::string& output){
    static std::mutex write_mutex;
    std::lock_guard lock(write_mutex);

    std::string buffer;
    for (uint i = 0; i < output.size(); i++){
        if (output[i] != '@'){
            buffer += output[i];
            continue;
        }
        bool is_color = false;
        for (const auto&[name, color] : COLOR_CODES){
            if (name.size() > output.size() - i){
                break;
            }
            if (std::equal(name.begin(), name.end(), output.begin() + i)){
                std::cout << buffer << std::flush;
                buffer.clear();
                set_console_color(color);
                is_color = true;
                i += name.size() - 1;
                break;
            }
        }
        if (!is_color){
            buffer += output[i];
        }
    }
    stream << buffer << '\n' << std::flush;
    set_console_color(OutputColor::RESET);
}

void Cycle::basic_writeln(const std::string& output){
    basic_writeln(std::cout, output);
}

void Cycle::exit_success() noexcept {
    Cycle::writeln(std::cerr, "{}[NOTICE]: Program exited successfully", GREY_COLOR);
    std::exit(EXIT_SUCCESS);
}

void Cycle::exit_failure() noexcept {
    Cycle::writeln(std::cerr, "{}[NOTICE]: Program exited with failure", GREY_COLOR);
    std::exit(EXIT_FAILURE);
}
