#include "HIR/value.hpp"
#include "Common/utils.hpp"
#include "Common/write.hpp"
#include <sstream>

using namespace Cycle::HIR;

// StructValueSet:

StructValueSet::StructValueSet(std::vector<std::unique_ptr<ValueSetAndType>>&& ordered_value_sets):
    _ordered_value_sets(std::move(ordered_value_sets))
{}

std::size_t StructValueSet::get_value_count() const {
    return _ordered_value_sets.size();
}

ValueSetAndType* StructValueSet::get_value_at_index(std::size_t index){
    return _ordered_value_sets.at(index).get();
}

const ValueSetAndType* StructValueSet::get_value_at_index(std::size_t index) const {
    return _ordered_value_sets.at(index).get();
}

std::string StructValueSet::print_value_set(uint indentation) const {
    if (_ordered_value_sets.empty()) return "{}";

    std::ostringstream oss;
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "{\n";

    uint counter = 0;
    for (const std::unique_ptr<ValueSetAndType>& value : _ordered_value_sets){
        for (uint i = 0; i < indentation + 1; i++) oss << TAB_SPACE;
        oss << "[struct field " << counter++ << "]: ";
        oss << value->get_value_set()->print_value_set(0) << '\n';
    }
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "}";
    return oss.str();
}

// PointerValueSet:

void PointerValueSet::add_pointed_value(ValueSet* pointed_value_set){
    _possible_pointed_values.push_back(pointed_value_set);
}

void PointerValueSet::add_null(){
    _can_be_null = true;
}

std::vector<ValueSet*>& PointerValueSet::get_possible_pointed_values(){
    return _possible_pointed_values;
}

const std::vector<ValueSet*>& PointerValueSet::get_possible_pointed_values() const {
    return _possible_pointed_values;
}

bool PointerValueSet::can_be_null() const {
    return _can_be_null;
}

bool PointerValueSet::is_always_null() const {
    return _can_be_null && _possible_pointed_values.empty();
}

std::string PointerValueSet::print_value_set(uint indentation) const {
    std::ostringstream oss;
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;

    if (_can_be_null){
        oss << "Pointer might be null or point to one of these set: {\n";
    } else {
        oss << "Pointer points to one of these set: {\n";
    }
    for (const ValueSet* value_set : _possible_pointed_values){
        for (uint i = 0; i < indentation + 1; i++) oss << TAB_SPACE;
        oss << "[Dereferenced]: ";
        oss << value_set->print_value_set(0);
    }
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "}";
    return oss.str();
}

// ValueSetAndType:

ValueSetAndType::ValueSetAndType(const Type* type, std::unique_ptr<ValueSet>&& value_set):
    _type(type),
    _value_set(std::move(value_set))
{}

const Type* ValueSetAndType::get_type() const {
    return _type;
}

const ValueSet* ValueSetAndType::get_value_set() const {
    return _value_set.get();
}
