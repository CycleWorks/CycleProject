#include "value.hpp"
#include "Common/utils.hpp"
#include <sstream>

using namespace Cycle;

// StructValueSet:

StructValueSet::StructValueSet(std::vector<std::unique_ptr<ValueSet>>&& ordered_value_sets):
    _ordered_value_sets(std::move(ordered_value_sets))
{}

std::size_t StructValueSet::get_value_count() const {
    return _ordered_value_sets.size();
}

ValueSet* StructValueSet::get_value_at_index(std::size_t index){
    return _ordered_value_sets.at(index).get();
}

const ValueSet* StructValueSet::get_value_at_index(std::size_t index) const {
    return _ordered_value_sets.at(index).get();
}

std::string StructValueSet::print_value_set(uint indentation) const {
    if (_ordered_value_sets.empty()) return "{}";

    std::ostringstream oss;
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "{\n";

    uint i = 0;
    for (const std::unique_ptr<ValueSet>& value_set : _ordered_value_sets){
        for (uint i = 0; i < indentation + 1; i++) oss << TAB_SPACE;
        oss << "[struct field " << i++ << "]: ";
        oss << value_set->print_value_set(0) << '\n';
    }
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "}";
    return oss.str();
}

// Value:

Value::Value(const Type* type, std::unique_ptr<ValueSet>&& value_set):
    _type(type),
    _value_set(std::move(value_set))
{}

Value::Value(const Type* type):
    _type(type),
    _value_set(type->create_default_value_set())
{}
