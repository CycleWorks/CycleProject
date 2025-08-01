#include "Symbols/symbol.hpp"

using namespace Cycle::Symbols;

Symbol::Symbol(const std::string& symbol_name):
    _symbol_name(symbol_name)
{}

std::string_view Symbol::get_symbol_name() const {
    return _symbol_name;
}
