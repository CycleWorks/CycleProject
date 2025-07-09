#pragma once

#include <streambuf>
#include <format>

namespace Cycle {
    void basic_writeln(std::ostream& stream, const std::string& output);
    void basic_writeln(const std::string& output);
    void exit_success() noexcept;
    void exit_failure() noexcept;

    template <typename... Args>
    constexpr void writeln(std::ostream& stream, const std::format_string<Args...>& fmt, Args&&... args){
        std::string msg = std::format(fmt, std::forward<Args>(args)...);
        basic_writeln(stream, msg);
    }
    template <typename... Args>
    constexpr void writeln(const std::format_string<Args...>& fmt, Args&&... args){
        std::string msg = std::format(fmt, std::forward<Args>(args)...);
        basic_writeln(msg);
    }
}
