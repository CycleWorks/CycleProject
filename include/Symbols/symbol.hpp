#pragma once

#include <string>

namespace Cycle::Symbols {
    struct Symbol {
        explicit Symbol(const std::string& symbol_name);
        std::string_view get_symbol_name() const;
    private:
        std::string _symbol_name;
    };
}
