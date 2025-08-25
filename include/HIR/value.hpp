#pragma once

#include "Common/utils.hpp"
#include "Common/numerics.hpp"
#include "type.hpp"
#include <cstdint>
#include <set>
#include <type_traits>
#include <unordered_set>

namespace Cycle::HIR {
    struct ValueSet {
        virtual std::string print_value_set(uint indentation = 0) const = 0;
        virtual ~ValueSet(){}
    };
    struct ValueSetAndType;

    struct StructValueSet : public ValueSet {
        explicit StructValueSet(std::vector<std::unique_ptr<ValueSetAndType>>&& ordered_value_sets);
        std::size_t get_value_count() const;
        ValueSetAndType* get_value_at_index(std::size_t index);
        const ValueSetAndType* get_value_at_index(std::size_t index) const;

        virtual std::string print_value_set(uint indentation = 0) const override;
    private:
        std::vector<std::unique_ptr<ValueSetAndType>> _ordered_value_sets;
    };

    template <typename T>
    requires IsNumeric<T>
    struct NumericValueSet : public ValueSet {
        struct Range {
            explicit Range(NumericWrapper<T> min, NumericWrapper<T> max, NumericWrapper<T> step);
            explicit Range(const Range&) = default;

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
    public:
        explicit NumericValueSet() = default;
        explicit NumericValueSet(const NumericValueSet&) = default;

        NumericValueSet& add_value(NumericWrapper<T> value);
        NumericValueSet& add_range(const Range& value);
        NumericValueSet& remove_value(NumericWrapper<T> value);
        NumericValueSet& remove_values_over(NumericWrapper<T> value);
        NumericValueSet& remove_values_under(NumericWrapper<T> value);
        bool contains(NumericWrapper<T> value) const;
        bool is_initialized() const;
        bool has_single_possibility() const;
        virtual std::string print_value_set(uint indentation = 0) const override;
    private:
        void _promote_values_to_range();
        void _merge_new_range(const Range& new_range);

        std::set<NumericWrapper<T>> _possible_values;
        std::vector<Range> _possible_ranges;
        bool _initialized = false;
    };

    struct ValueSetAndType {
        explicit ValueSetAndType(const Type* type, std::unique_ptr<ValueSet>&& value_set);
        const Type* get_type() const;
        const ValueSet* get_value_set() const;
    private:
        const Type* _type;
        std::unique_ptr<ValueSet> _value_set;
    };

    // Factories:
    struct StructValueSetFactory : public BasicFactory<StructValueSet> {};
    template <typename T>
    struct NumberValueSetFactory : public BasicFactory<NumericValueSet<T>> {};
    struct ValueSetAndTypeFactory : public BasicFactory<ValueSetAndType> {};
}

#include "value.tpp"
