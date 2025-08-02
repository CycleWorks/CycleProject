#pragma once

#include "Common/utils.hpp"
#include "Symbols/symbol.hpp"
#include <memory>
#include <span>
#include <string_view>
#include <unordered_map>

namespace Cycle {
    struct ValueSet;

    struct Type {
        virtual std::unique_ptr<ValueSet> create_default_value_set() const = 0;
    };
    
    struct NumericType : public Type {
        enum class Kind;

        static const NumericType* get_i8();
        static const NumericType* get_i16();
        static const NumericType* get_i32();
        static const NumericType* get_i64();
        static const NumericType* get_u8();
        static const NumericType* get_u16();
        static const NumericType* get_u32();
        static const NumericType* get_u64();
        static const NumericType* get_float();
        static const NumericType* get_double();

        std::unique_ptr<ValueSet> create_default_value_set() const override;
    private:
        NumericType(NumericType::Kind kind);
        NumericType::Kind _kind;
    };

    struct UnnamedStructType : public Type {
        static const UnnamedStructType* get_struct(const std::vector<const Type*>& ordered_field_types);
        std::span<const Type* const> get_ordered_field_types() const;

        std::unique_ptr<ValueSet> create_default_value_set() const override;
    private:
        UnnamedStructType(const std::vector<const Type*>& ordered_field_types);
        std::vector<const Type*> _ordered_field_types;
    };
}

enum class Cycle::NumericType::Kind {
    INT8 = 1,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE
};
