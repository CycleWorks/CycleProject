#include "Common/errors.hpp"
#include "Common/colors.hpp"
#include <format>

using namespace Cycle;

ThrowableException::ThrowableException(ColorNameView error_name_color, const std::string& error_name, const std::string& error_message):
    _error_message(std::format(
        "[{}{}{}]: {}",
        error_name_color, error_name, RESET_COLOR, error_message
    ))
{}

const char* ThrowableException::what() const {
    return _error_message.c_str();
}
