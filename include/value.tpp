#pragma once

#include "Common/errors.hpp"
#include "Common/numerics.hpp"
#include "Common/math.hpp"
#include "Common/utils.hpp"
#include "Common/write.hpp"
#include "value.hpp"
#include <algorithm>
#include <complex>
#include <format>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <cstdint>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace Cycle {
    // NumberValueSet<T>::Range:

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>::Range::Range(NumericWrapper<T> min, NumericWrapper<T> max, NumericWrapper<T> step): _min(min), _max(max), _step(step){
        if (_step == NumericWrapper<T>(0)){
            throw InternalError("NumberValueSet<T> failed: step cannot be 0");
        }
        if (_min > _max) std::swap(_min, _max);
        _step = absolute(_step);
        _max -= (_max - _min) % _step;
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::Range::set_min(NumericWrapper<T> min){ *this = Range(min, _max, _step); }
    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::Range::set_max(NumericWrapper<T> max){ *this = Range(_min, max, _step); }
    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::Range::set_step(NumericWrapper<T> step){ *this = Range(_min, _max, step); }

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> NumberValueSet<T>::Range::get_min() const { return _min; }
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> NumberValueSet<T>::Range::get_max() const { return _max; }
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> NumberValueSet<T>::Range::get_step() const { return _step; }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::Range::contains(NumericWrapper<T> value) const {
        if (value < _min || value > _max) return false;
        NumericWrapper<long double> rem = (value - _min) % _step;
        return rem == NumericWrapper<long double>(0);
    }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::Range::try_merge_with(const Range& other){
        if (_step != other.get_step()) return false;

        // If previous step, append to eachother
        if (!subtraction_will_overflow(_min, _step)){
            if (_min - _step == other.get_max()){
                _merge_with(other);
                return true;
            }
        }
        // If next step, append to eachother
        if (!addition_will_overflow(_max, _step)){
            if (_max + _step == other.get_min()){
                _merge_with(other);
                return true;
            }
        }
        // If out of range
        if (_min > other.get_max() || _max < other.get_min()){
            return false;
        }
        // Check if there's an overlap
        NumericWrapper<T> smaller_min = std::min(_min, other.get_min());
        NumericWrapper<T> bigger_min = std::max(_min, other.get_min());
        if ((bigger_min - smaller_min) % _step == NumericWrapper<T>(0)){
            _merge_with(other);
            return true;
        }
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::Range::_merge_with(const Range& other){
        _min = std::min(_min, other.get_min());
        _max = std::max(_max, other.get_max());
        *this = Range(_min, _max, _step);
    }

    // NumberValueSet<T>:

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>& NumberValueSet<T>::add_value(NumericWrapper<T> value){
        _possible_values.insert(value);

        for (Range& range : _possible_ranges){
            Range new_range(value, value, range.get_step());
            if (range.try_merge_with(new_range)) return *this;
        }
        _promote_values_to_range();
        _initialized = true;
        return *this;
    }

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>& NumberValueSet<T>::add_range(const Range& value){
        _merge_new_range(value);
        _initialized = true;
        return *this;
    }

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>& NumberValueSet<T>::remove_value(NumericWrapper<T> value){
        _possible_values.erase(value);

        std::vector<Range> previous_ranges = _possible_ranges;
        _possible_ranges.clear();

        for (const Range& range : previous_ranges){
            if (!range.contains(value)){
                _merge_new_range(range);
                continue;
            }
            if (range.get_min() == range.get_max()){
                continue;
            }
            if (value == range.get_min()){
                Range new_range(range.get_min() + range.get_step(), range.get_max(), range.get_step());
                _merge_new_range(new_range);
                continue;
            }
            if (value == range.get_max()){
                Range new_range(range.get_min(), range.get_max() - range.get_step(), range.get_step());
                _merge_new_range(new_range);
                continue;
            }
            // Cut range in half
            Range range_a(range.get_min(), value - range.get_step(), range.get_step());
            Range range_b(value + range.get_step(), range.get_max(), range.get_step());

            _merge_new_range(range_a);
            _merge_new_range(range_b);
        }
        return *this;
    }

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>& NumberValueSet<T>::remove_values_over(NumericWrapper<T> value){
        std::set<NumericWrapper<T>> new_possible_value;

        for (const NumericWrapper<T>& possible_value : _possible_values){
            if (possible_value <= value) new_possible_value.insert(possible_value);
        }
        _possible_values = std::move(new_possible_value);

        std::vector<Range> previous_ranges = _possible_ranges;
        _possible_ranges.clear();

        for (const Range& range : previous_ranges){
            if (value >= range.get_max()){
                _merge_new_range(range);
                continue;
            }
            if (value < range.get_min()){
                continue;
            }
            NumericWrapper<T> steps = (T)floor_division(value - range.get_min(), range.get_step()).value();
            NumericWrapper<T> max_value = range.get_min() + steps * range.get_step();
            if (max_value >= range.get_min()){
                Range new_range(range.get_min(), NumericWrapper<T>(max_value.value()), range.get_step());
                _merge_new_range(new_range);
            }
        }
        return *this;
    }

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>& NumberValueSet<T>::remove_values_under(NumericWrapper<T> value){
        std::set<NumericWrapper<T>> new_possible_value;

        for (const NumericWrapper<T>& possible_value : _possible_values){
            if (possible_value >= value) new_possible_value.insert(possible_value);
        }
        _possible_values = std::move(new_possible_value);

        std::vector<Range> previous_ranges = _possible_ranges;
        _possible_ranges.clear();

        for (const Range& range : previous_ranges){
            if (value <= range.get_min()){
                _merge_new_range(range);
                continue;
            }
            if (value > range.get_max()){
                continue;
            }
            NumericWrapper<T> steps = (T)ceil_division(value - range.get_min(), range.get_step()).value();
            NumericWrapper<T> min_value = range.get_min() + steps * range.get_step();
            if (min_value <= range.get_max()){
                Range new_range(min_value, range.get_max(), range.get_step());
                _merge_new_range(new_range);
            }
        }
        return *this;
    }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::contains(NumericWrapper<T> value) const {
        if (_possible_values.contains(value)) return true;

        for (const Range& possible_range : _possible_ranges){
            if (possible_range.contains(value)) return true;
        }
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::is_initialized() const {
        return _initialized;
    }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::has_single_possibility() const {
        return _possible_values.size() + _possible_ranges.size() == 1;
    }

    template <typename T>
    requires IsNumeric<T>
    std::string NumberValueSet<T>::print_value_set(uint indentation) const {
        std::ostringstream oss;
        for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
        oss << "{";
        for (auto it = _possible_values.begin(); it != _possible_values.end(); it++){
            if constexpr (sizeof(T) == 1){
                oss << (int)(T)*it;
            } else {
                oss << (T)*it;
            }
            if (std::next(it) != _possible_values.end()){
                oss << ", ";
            }
        }
        oss << "}";
        for (const Range& range : _possible_ranges){
            oss << " && ";
            oss << std::format("[{}, {}, {}]", range.get_min(), range.get_max(), range.get_step());
        }
        return oss.str();
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::_promote_values_to_range(){
        if (_possible_values.size() < 3) return;

        std::vector<T> sorted(_possible_values.begin(), _possible_values.end());
        std::set<T> values_to_remove;

        for (std::size_t i = 0; i < sorted.size() - 2; i++){
            std::size_t initial_index = i;

            NumericWrapper<T> first_value = sorted[i];
            NumericWrapper<T> second_value = sorted[i + 1];
            NumericWrapper<T> third_value = sorted[i + 2];

            NumericWrapper<T> first_step = second_value - first_value;
            NumericWrapper<T> second_step = third_value - second_value;

            if (first_step != second_step){
                continue;
            }
            NumericWrapper<T> min = first_value;
            NumericWrapper<T> max = third_value;
            NumericWrapper<T> step = first_step;

            i += 3;
            while (i < sorted.size()){
                NumericWrapper<T> new_step = sorted[i] - sorted[i - 1];
                if (step != new_step) break;
                max = sorted[i];
                i++;
            }
            Range range(min, max, step);
            _merge_new_range(range);

            for (std::size_t j = initial_index; j < i; j++){
                values_to_remove.insert(sorted[j]);
            }
            i -= 1;
        }
        for (T value : values_to_remove){
            _possible_values.erase(value);
        }
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::_merge_new_range(const Range& new_range){
        for (Range& range : _possible_ranges){
            if (range.try_merge_with(new_range)) return;
        }
        // If only one value
        if (new_range.get_min() == new_range.get_max()){
            _possible_values.insert(new_range.get_min());
            return;
        }
        // If only two value
        if (new_range.get_min() + new_range.get_step() == new_range.get_max()){
            _possible_values.insert(new_range.get_min());
            _possible_values.insert(new_range.get_max());
            return;
        }
        _possible_ranges.push_back(new_range);
    }
}
