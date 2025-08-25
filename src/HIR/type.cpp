#include "HIR/type.hpp"
#include "HIR/value.hpp"
#include <limits>
#include <memory>
#include <stdint.h>
#include <vector>

using namespace Cycle::HIR;

// NumericType:

const NumericType* NumericType::get_i8(){
    static const NumericType singleton(NumericType::Kind::INT8);
    return &singleton;
}

const NumericType* NumericType::get_i16(){
    static const NumericType singleton(NumericType::Kind::INT16);
    return &singleton;
}

const NumericType* NumericType::get_i32(){
    static const NumericType singleton(NumericType::Kind::INT32);
    return &singleton;
}

const NumericType* NumericType::get_i64(){
    static const NumericType singleton(NumericType::Kind::INT64);
    return &singleton;
}

const NumericType* NumericType::get_u8(){
    static const NumericType singleton(NumericType::Kind::UINT8);
    return &singleton;
}

const NumericType* NumericType::get_u16(){
    static const NumericType singleton(NumericType::Kind::UINT16);
    return &singleton;
}

const NumericType* NumericType::get_u32(){
    static const NumericType singleton(NumericType::Kind::UINT32);
    return &singleton;
}

const NumericType* NumericType::get_u64(){
    static const NumericType singleton(NumericType::Kind::UINT64);
    return &singleton;
}

const NumericType* NumericType::get_float(){
    static const NumericType singleton(NumericType::Kind::FLOAT);
    return &singleton;
}

const NumericType* NumericType::get_double(){
    static const NumericType singleton(NumericType::Kind::DOUBLE);
    return &singleton;
}

std::unique_ptr<ValueSetAndType> NumericType::create_uninitialized_value() const {
    static NumericValueSet<int8_t> single_i8;
    static NumericValueSet<int16_t> single_i16;
    static NumericValueSet<int32_t> single_i32;
    static NumericValueSet<int64_t> single_i64;
    static NumericValueSet<uint8_t> single_u8;
    static NumericValueSet<uint16_t> single_u16;
    static NumericValueSet<uint32_t> single_u32;
    static NumericValueSet<uint64_t> single_u64;
    static NumericValueSet<float> single_float;
    static NumericValueSet<double> single_double;

    std::unique_ptr<ValueSet> value_set;
    switch (_kind){
    case NumericType::Kind::INT8:
        value_set = std::unique_ptr<NumericValueSet<int8_t>>(new NumericValueSet<int8_t>(single_i8));
        break;
    case NumericType::Kind::INT16:
        value_set = std::unique_ptr<NumericValueSet<int16_t>>(new NumericValueSet<int16_t>(single_i16));
        break;
    case NumericType::Kind::INT32:
        value_set = std::unique_ptr<NumericValueSet<int32_t>>(new NumericValueSet<int32_t>(single_i32));
        break;
    case NumericType::Kind::INT64:
        value_set = std::unique_ptr<NumericValueSet<int64_t>>(new NumericValueSet<int64_t>(single_i64));
        break;
    case NumericType::Kind::UINT8:
        value_set = std::unique_ptr<NumericValueSet<uint8_t>>(new NumericValueSet<uint8_t>(single_u8));
        break;
    case NumericType::Kind::UINT16:
        value_set = std::unique_ptr<NumericValueSet<uint16_t>>(new NumericValueSet<uint16_t>(single_u16));
        break;
    case NumericType::Kind::UINT32:
        value_set = std::unique_ptr<NumericValueSet<uint32_t>>(new NumericValueSet<uint32_t>(single_u32));
        break;
    case NumericType::Kind::UINT64:
        value_set = std::unique_ptr<NumericValueSet<uint64_t>>(new NumericValueSet<uint64_t>(single_u64));
        break;
    case NumericType::Kind::FLOAT:
        value_set = std::unique_ptr<NumericValueSet<float>>(new NumericValueSet<float>(single_float));
        break;
    case NumericType::Kind::DOUBLE:
        value_set = std::unique_ptr<NumericValueSet<double>>(new NumericValueSet<double>(single_double));
        break;
    }
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

std::unique_ptr<ValueSetAndType> NumericType::create_default_value() const {
    static NumericValueSet<int8_t> single_i8;
    static NumericValueSet<int16_t> single_i16;
    static NumericValueSet<int32_t> single_i32;
    static NumericValueSet<int64_t> single_i64;
    static NumericValueSet<uint8_t> single_u8;
    static NumericValueSet<uint16_t> single_u16;
    static NumericValueSet<uint32_t> single_u32;
    static NumericValueSet<uint64_t> single_u64;
    static NumericValueSet<float> single_float;
    static NumericValueSet<double> single_double;
    static bool initialized = false;

    if (!initialized){
        single_i8.add_value(0);
        single_i16.add_value(0);
        single_i32.add_value(0);
        single_i64.add_value(0);
        single_u8.add_value(0);
        single_u16.add_value(0);
        single_u32.add_value(0);
        single_u64.add_value(0);
        single_float.add_value(0);
        single_double.add_value(0);
        initialized = true;
    }
    std::unique_ptr<ValueSet> value_set;
    switch (_kind){
    case NumericType::Kind::INT8:
        value_set = std::unique_ptr<NumericValueSet<int8_t>>(new NumericValueSet<int8_t>(single_i8));
        break;
    case NumericType::Kind::INT16:
        value_set = std::unique_ptr<NumericValueSet<int16_t>>(new NumericValueSet<int16_t>(single_i16));
        break;
    case NumericType::Kind::INT32:
        value_set = std::unique_ptr<NumericValueSet<int32_t>>(new NumericValueSet<int32_t>(single_i32));
        break;
    case NumericType::Kind::INT64:
        value_set = std::unique_ptr<NumericValueSet<int64_t>>(new NumericValueSet<int64_t>(single_i64));
        break;
    case NumericType::Kind::UINT8:
        value_set = std::unique_ptr<NumericValueSet<uint8_t>>(new NumericValueSet<uint8_t>(single_u8));
        break;
    case NumericType::Kind::UINT16:
        value_set = std::unique_ptr<NumericValueSet<uint16_t>>(new NumericValueSet<uint16_t>(single_u16));
        break;
    case NumericType::Kind::UINT32:
        value_set = std::unique_ptr<NumericValueSet<uint32_t>>(new NumericValueSet<uint32_t>(single_u32));
        break;
    case NumericType::Kind::UINT64:
        value_set = std::unique_ptr<NumericValueSet<uint64_t>>(new NumericValueSet<uint64_t>(single_u64));
        break;
    case NumericType::Kind::FLOAT:
        value_set = std::unique_ptr<NumericValueSet<float>>(new NumericValueSet<float>(single_float));
        break;
    case NumericType::Kind::DOUBLE:
        value_set = std::unique_ptr<NumericValueSet<double>>(new NumericValueSet<double>(single_double));
        break;
    }
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

std::unique_ptr<ValueSetAndType> NumericType::create_worst_value() const {
    static NumericValueSet<int8_t> single_i8;
    static NumericValueSet<int16_t> single_i16;
    static NumericValueSet<int32_t> single_i32;
    static NumericValueSet<int64_t> single_i64;
    static NumericValueSet<uint8_t> single_u8;
    static NumericValueSet<uint16_t> single_u16;
    static NumericValueSet<uint32_t> single_u32;
    static NumericValueSet<uint64_t> single_u64;
    static NumericValueSet<float> single_float;
    static NumericValueSet<double> single_double;
    static bool initialized = false;

    if (!initialized){
        single_i8.add_range(NumericValueSet<int8_t>::Range(NumericWrapper<int8_t>::min(), NumericWrapper<int8_t>::max(), 1));
        single_i16.add_range(NumericValueSet<int16_t>::Range(NumericWrapper<int16_t>::min(), NumericWrapper<int16_t>::max(), 1));
        single_i32.add_range(NumericValueSet<int32_t>::Range(NumericWrapper<int32_t>::min(), NumericWrapper<int32_t>::max(), 1));
        single_i64.add_range(NumericValueSet<int64_t>::Range(NumericWrapper<int64_t>::min(), NumericWrapper<int64_t>::max(), 1));
        single_u8.add_range(NumericValueSet<uint8_t>::Range(NumericWrapper<uint8_t>::min(), NumericWrapper<uint8_t>::max(), 1));
        single_u16.add_range(NumericValueSet<uint16_t>::Range(NumericWrapper<uint16_t>::min(), NumericWrapper<uint16_t>::max(), 1));
        single_u32.add_range(NumericValueSet<uint32_t>::Range(NumericWrapper<uint32_t>::min(), NumericWrapper<uint32_t>::max(), 1));
        single_u64.add_range(NumericValueSet<uint64_t>::Range(NumericWrapper<uint64_t>::min(), NumericWrapper<uint64_t>::max(), 1));
        single_float.add_range(NumericValueSet<float>::Range(NumericWrapper<float>::min(), NumericWrapper<float>::max(), 1));
        single_double.add_range(NumericValueSet<double>::Range(NumericWrapper<double>::min(), NumericWrapper<double>::max(), 1));
        initialized = true;
    }
    std::unique_ptr<ValueSet> value_set;
    switch (_kind){
    case NumericType::Kind::INT8:
        value_set = std::unique_ptr<NumericValueSet<int8_t>>(new NumericValueSet<int8_t>(single_i8));
        break;
    case NumericType::Kind::INT16:
        value_set = std::unique_ptr<NumericValueSet<int16_t>>(new NumericValueSet<int16_t>(single_i16));
        break;
    case NumericType::Kind::INT32:
        value_set = std::unique_ptr<NumericValueSet<int32_t>>(new NumericValueSet<int32_t>(single_i32));
        break;
    case NumericType::Kind::INT64:
        value_set = std::unique_ptr<NumericValueSet<int64_t>>(new NumericValueSet<int64_t>(single_i64));
        break;
    case NumericType::Kind::UINT8:
        value_set = std::unique_ptr<NumericValueSet<uint8_t>>(new NumericValueSet<uint8_t>(single_u8));
        break;
    case NumericType::Kind::UINT16:
        value_set = std::unique_ptr<NumericValueSet<uint16_t>>(new NumericValueSet<uint16_t>(single_u16));
        break;
    case NumericType::Kind::UINT32:
        value_set = std::unique_ptr<NumericValueSet<uint32_t>>(new NumericValueSet<uint32_t>(single_u32));
        break;
    case NumericType::Kind::UINT64:
        value_set = std::unique_ptr<NumericValueSet<uint64_t>>(new NumericValueSet<uint64_t>(single_u64));
        break;
    case NumericType::Kind::FLOAT:
        value_set = std::unique_ptr<NumericValueSet<float>>(new NumericValueSet<float>(single_float));
        break;
    case NumericType::Kind::DOUBLE:
        value_set = std::unique_ptr<NumericValueSet<double>>(new NumericValueSet<double>(single_double));
        break;
    }
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

NumericType::NumericType(NumericType::Kind kind):
    _kind(kind)
{}

// UnnamedStructType:

const UnnamedStructType* UnnamedStructType::get_struct(const std::vector<const Type*>& ordered_field_types){
    static std::vector<UnnamedStructType> struct_singletons;
    const UnnamedStructType* ret = nullptr;

    for (const UnnamedStructType& other : struct_singletons){
        std::span<const Type *const> other_ordered_field_types = other.get_ordered_field_types();
        if (ordered_field_types.size() != other_ordered_field_types.size()){
            continue;
        }
        for (std::size_t i = 0; i < ordered_field_types.size(); i++){
            if (ordered_field_types[i] != other_ordered_field_types[i]){
                ret = &other;
                break;
            }
        }
        if (ret != nullptr){
            break;
        }
    }
    if (ret == nullptr){
        struct_singletons.push_back(UnnamedStructType(ordered_field_types));
        ret = &struct_singletons.back();
    }
    return ret;
}

std::span<const Type* const> UnnamedStructType::get_ordered_field_types() const {
    return std::span<const Type* const>(_ordered_field_types.data(), _ordered_field_types.size());;
}

std::unique_ptr<ValueSetAndType> UnnamedStructType::create_uninitialized_value() const {
    std::vector<std::unique_ptr<ValueSetAndType>> ordered_types;
    for (const Type* const field : _ordered_field_types){
        ordered_types.push_back(field->create_uninitialized_value());
    }
    std::unique_ptr<ValueSet> value_set(new StructValueSet(std::move(ordered_types)));
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

std::unique_ptr<ValueSetAndType> UnnamedStructType::create_default_value() const {
    std::vector<std::unique_ptr<ValueSetAndType>> ordered_types;
    for (const Type* const field : _ordered_field_types){
        ordered_types.push_back(field->create_default_value());
    }
    std::unique_ptr<ValueSet> value_set(new StructValueSet(std::move(ordered_types)));
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

std::unique_ptr<ValueSetAndType> UnnamedStructType::create_worst_value() const {
    std::vector<std::unique_ptr<ValueSetAndType>> ordered_types;
    for (const Type* const field : _ordered_field_types){
        ordered_types.push_back(field->create_worst_value());
    }
    std::unique_ptr<ValueSet> value_set(new StructValueSet(std::move(ordered_types)));
    return std::unique_ptr<ValueSetAndType>(new ValueSetAndType(this, std::move(value_set)));
}

UnnamedStructType::UnnamedStructType(const std::vector<const Type*>& ordered_field_types):
    _ordered_field_types(ordered_field_types)
{}
