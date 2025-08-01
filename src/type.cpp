#include "type.hpp"
#include <vector>

using namespace Cycle;

// PrimitiveType:

const PrimitiveType* PrimitiveType::get_i8(){
    static const PrimitiveType singleton(PrimitiveType::Kind::INT8);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_i16(){
    static const PrimitiveType singleton(PrimitiveType::Kind::INT16);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_i32(){
    static const PrimitiveType singleton(PrimitiveType::Kind::INT32);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_i64(){
    static const PrimitiveType singleton(PrimitiveType::Kind::INT64);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_u8(){
    static const PrimitiveType singleton(PrimitiveType::Kind::UINT8);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_u16(){
    static const PrimitiveType singleton(PrimitiveType::Kind::UINT16);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_u32(){
    static const PrimitiveType singleton(PrimitiveType::Kind::UINT32);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_u64(){
    static const PrimitiveType singleton(PrimitiveType::Kind::UINT64);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_float(){
    static const PrimitiveType singleton(PrimitiveType::Kind::FLOAT);
    return &singleton;
}

const PrimitiveType* PrimitiveType::get_double(){
    static const PrimitiveType singleton(PrimitiveType::Kind::DOUBLE);
    return &singleton;
}

PrimitiveType::PrimitiveType(PrimitiveType::Kind kind):
    _kind(kind)
{}

// UnnamedStructType:

const UnnamedStructType* UnnamedStructType::get_struct(const std::vector<Type*>& field_types){
    static std::vector<UnnamedStructType> structs;
    const UnnamedStructType* ret = nullptr;

    for (const UnnamedStructType& other : structs){
        auto& other_field_types = other.get_field_types();
        if (field_types.size() != other_field_types.size()){
            continue;
        }
        for (std::size_t i = 0; i < field_types.size(); i++){
            if (field_types[i] != other_field_types[i]){
                ret = &other;
                break;
            }
        }
        if (ret != nullptr){
            break;
        }
    }
    if (ret == nullptr){
        structs.push_back(UnnamedStructType(field_types));
        ret = &structs.back();
    }
    return ret;
}

const std::vector<Type*>& UnnamedStructType::get_field_types() const {
    return _field_types;
}

UnnamedStructType::UnnamedStructType(const std::vector<Type*>& field_types):
    _field_types(field_types)
{}
