#pragma once

#include "colors.hpp"
#include <exception>
#include <format>
#include <string>

namespace Cycle {
    struct ThrowableException : public std::exception {
        ThrowableException(const ColorName& error_name_color, const std::string& error_name, const std::string& error_message);
        const char* what() const override;
    private:
        const std::string _error_message;
    };

    struct InternalError : public ThrowableException {
        template <typename...Args>
        InternalError(const std::format_string<Args...> fmt, Args... args):
            ThrowableException(RED_COLOR, "InternalError", std::format(fmt, std::forward<Args>(args)...))
        {}
    };
}
