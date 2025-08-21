#pragma once

#include "Common/colors.hpp"
#include <iostream>
#include <format>
#include <utility>
#include <thread>

namespace Cycle {
    void basic_writeln(std::ostream& stream, const std::string& output);
    void exit_success() noexcept;
    void exit_failure() noexcept;

    template <typename... Args>
    constexpr void writeln(std::ostream& stream, const std::format_string<Args...>& fmt, Args&&... args){
        std::string msg = std::format(fmt, std::forward<Args>(args)...);
        basic_writeln(stream, msg);
    }
    template <typename... Args>
    constexpr void writeln(const std::format_string<Args...>& fmt, Args&&... args){
        writeln(std::cout, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    constexpr void warningln(std::ostream& stream, const std::format_string<Args...>& fmt, Args&&... args){
        std::string msg = std::format(fmt, std::forward<Args>(args)...);
        msg = std::format("{}[WARNING IN THREAD #{}]: {}{}", GREY_COLOR, std::this_thread::get_id(), msg, RESET_COLOR);
        basic_writeln(stream, msg);
    }
    template <typename... Args>
    constexpr void warningln(const std::format_string<Args...>& fmt, Args&&... args){
        warningln(std::cout, fmt, std::forward<Args>(args)...);
    }
}
