#pragma once

#include "Common/utils.hpp"
#include "type.hpp"
#include <cstdint>
#include <set>
#include <type_traits>

namespace Cycle {
    struct ValueSet {
        virtual std::string print_value_set(uint indentation = 0) const = 0;
        virtual ~ValueSet(){}
    };

    struct StructValueSet : public ValueSet {
        explicit StructValueSet(std::vector<std::unique_ptr<ValueSet>>&& ordered_value_sets);
        std::size_t get_value_count() const;
        ValueSet* get_value_at_index(std::size_t index);
        const ValueSet* get_value_at_index(std::size_t index) const;

        virtual std::string print_value_set(uint indentation = 0) const override;
    private:
        std::vector<std::unique_ptr<ValueSet>> _ordered_value_sets;
    };

    // TODO Add range deletion
    template <typename T>
    requires IsNumeric<T>
    struct NumberValueSet : public ValueSet {
        struct Range {
            Range(T min, T max, T step);
            void set_min(T min);
            void set_max(T max);
            void set_step(T step);
            T get_min() const;
            T get_max() const;
            T get_step() const;

            bool contains(T value) const;
            bool try_merge_with(const Range& other); // Returns true if merged
        private:
            void _merge_with(const Range& other);
            T _min;
            T _max;
            T _step;
        };
        void add_value(T value);
        void add_range(const Range& value);
        bool contains(T value) const;
        virtual std::string print_value_set(uint indentation = 0) const override;
    private:
        void _promote_values_to_range();
        void _merge_new_range(const Range& new_range);

        std::set<T> _possible_values;
        std::vector<Range> _possible_ranges;
    };

    struct Value {
        explicit Value(const Type* type, std::unique_ptr<ValueSet>&& value_set);
        explicit Value(const Type* type);
    private:
        const Type* _type;
        std::unique_ptr<ValueSet> _value_set;
    };

    // Factories:
    struct StructValueSetFactory : public BasicFactory<StructValueSet> {};
    template <typename T>
    struct NumberValueSetFactory : public BasicFactory<NumberValueSet<T>> {};
    struct ValueFactory : public BasicFactory<Value> {};
}

#include "value.tpp"
