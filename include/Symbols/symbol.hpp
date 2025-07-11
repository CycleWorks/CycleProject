#pragma once

#include <string>

namespace Cycle::Symbols {
    struct BaseSymbol {
        explicit BaseSymbol(const std::string& symbol_name);
        const std::string& get_symbol_name() const;
    private:
        std::string _symbol_name;
    };
}
