#pragma once

#include "Common/numerics.hpp"
#include "Common/utils.hpp"
#include "value.hpp"
#include <algorithm>
#include <complex>
#include <format>
#include <memory>
#include <sstream>
#include <cstdint>

namespace Cycle {
    // NumberValueSet<T>::Range:

    template <typename T>
    requires IsNumeric<T>
    NumberValueSet<T>::Range::Range(NumericWrapper<T> min, NumericWrapper<T> max, NumericWrapper<T> step): _min(min), _max(max), _step(step){
        if (_min > _max){
            std::swap(_min, _max);
        }
        _step = absolute((T)_step);
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
        if (_min - _step == other.get_max()){
            _merge_with(other);
            return true;
        }
        // If next step, append to eachother
        if (_max + _step == other.get_min()){
            _merge_with(other);
            return true;
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
    void NumberValueSet<T>::add_value(NumericWrapper<T> value){
        for (Range& range : _possible_ranges){
            Range new_range(value, value, range.get_step());
            if (range.try_merge_with(new_range)) return;
        }
        _possible_values.insert(value);
        _promote_values_to_range();
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::add_range(const Range& value){
        _merge_new_range(value);
    }

    template <typename T>
    requires IsNumeric<T>
    bool NumberValueSet<T>::contains(T value) const {
        if (_possible_values.contains(value)) return true;

        for (const Range& possible_range : _possible_ranges){
            if (possible_range.contains(value)) return true;
        }
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    std::string NumberValueSet<T>::print_value_set(uint indentation) const {
        std::ostringstream oss;
        for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
        oss << "{";
        for (auto it = _possible_values.begin(); it != _possible_values.end(); it++){
            oss << (T)*it;
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
        // Try to fit remaining values into new ranges
        std::vector<NumericWrapper<T>> leftover(_possible_values.begin(), _possible_values.end());
        std::vector<NumericWrapper<T>> still_unmerged;

        for (const NumericWrapper<T>& value : leftover){
            bool merged = false;
            for (Range& range : _possible_ranges){
                Range new_range(value, value, range.get_step());
                if (range.try_merge_with(new_range)){
                    merged = true;
                    break;
                }
            }
            if (merged){
                _possible_values.erase(value);
            }
        }
    }

    template <typename T>
    requires IsNumeric<T>
    void NumberValueSet<T>::_merge_new_range(const Range& new_range){
        std::vector<Range> merged_ranges;
        Range to_merge = new_range;

        auto it = _possible_ranges.begin();
        while (it != _possible_ranges.end()){
            if (to_merge.try_merge_with(*it)){
                it = _possible_ranges.erase(it); // remove and continue merging
                continue;
            }
            it++;
        }
        _possible_ranges.push_back(to_merge);
    }
}
