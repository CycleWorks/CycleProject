#include "Symbols/symbol.hpp"

using namespace Cycle::Symbols;

BaseSymbol::BaseSymbol(const std::string& symbol_name):
    _symbol_name(symbol_name)
{}

const std::string& BaseSymbol::get_symbol_name() const {
    return _symbol_name;
}
