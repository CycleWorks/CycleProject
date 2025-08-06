#pragma once

#include "Common/utils.hpp"
#include "Common/numerics.hpp"
#include "type.hpp"
#include <cstdint>
#include <set>
#include <type_traits>
#include <unordered_set>

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

    template <typename T>
    requires IsNumeric<T>
    struct NumberValueSet : public ValueSet {
        struct Range {
            Range(NumericWrapper<T> min, NumericWrapper<T> max, NumericWrapper<T> step);
            void set_min(NumericWrapper<T> min);
            void set_max(NumericWrapper<T> max);
            void set_step(NumericWrapper<T> step);
            NumericWrapper<T> get_min() const;
            NumericWrapper<T> get_max() const;
            NumericWrapper<T> get_step() const;

            bool contains(NumericWrapper<T> value) const;
            bool try_merge_with(const Range& other); // Returns true if merged
        private:
            void _merge_with(const Range& other);
            NumericWrapper<T> _min;
            NumericWrapper<T> _max;
            NumericWrapper<T> _step;
        };
        void add_value(NumericWrapper<T> value);
        void add_range(const Range& value);
        void remove_value(NumericWrapper<T> value);
        void remove_values_over(NumericWrapper<T> value);
        void remove_values_under(NumericWrapper<T> value);
        bool contains(NumericWrapper<T> value) const;
        bool has_single_possibility() const;
        virtual std::string print_value_set(uint indentation = 0) const override;
    private:
        void _promote_values_to_range();
        void _merge_new_range(const Range& new_range);

        std::set<NumericWrapper<T>> _possible_values;
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
