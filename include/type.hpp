#pragma once

#include "Common/utils.hpp"
#include "Symbols/symbol.hpp"
#include <memory>
#include <string_view>
#include <unordered_map>

namespace Cycle {
    struct Type : public AbstractClass {};
    
    struct PrimitiveType : public Type {
        enum class Kind;
        
        static const PrimitiveType* get_i8();
        static const PrimitiveType* get_i16();
        static const PrimitiveType* get_i32();
        static const PrimitiveType* get_i64();
        static const PrimitiveType* get_u8();
        static const PrimitiveType* get_u16();
        static const PrimitiveType* get_u32();
        static const PrimitiveType* get_u64();
        static const PrimitiveType* get_float();
        static const PrimitiveType* get_double();
    private:
        PrimitiveType(PrimitiveType::Kind kind);
        PrimitiveType::Kind _kind;

        void polymorphism() const override {}
    };

    struct UnnamedStructType : public Type {
        static const UnnamedStructType* get_struct(const std::vector<Type*>& field_types);
        const std::vector<Type*>& get_field_types() const;
    private:
        UnnamedStructType(const std::vector<Type*>& field_types);
        std::vector<Type*> _field_types;

        void polymorphism() const override {}
    };
}

enum class Cycle::PrimitiveType::Kind {
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
