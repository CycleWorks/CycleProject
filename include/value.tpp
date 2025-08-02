#pragma once

#include "Common/utils.hpp"
#include "value.hpp"
#include <algorithm>
#include <complex>
#include <format>
#include <memory>
#include <sstream>
#include <cstdint>

// NumberValueSet<T>::Range:

template <typename T>
requires Cycle::IsNumeric<T>
Cycle::NumberValueSet<T>::Range::Range(T min, T max, T step): _min(min), _max(max), _step(step){
    if (_min > _max){
        std::swap(_min, _max);
    }
    _step = absolute(_step);
    _max -= modulus(_max - _min, _step);
}

template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::Range::set_min(T min){ *this = Range(min, _max, _step); }
template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::Range::set_max(T max){ *this = Range(_min, max, _step); }
template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::Range::set_step(T step){ *this = Range(_min, _max, step); }

template <typename T>
requires Cycle::IsNumeric<T>
T Cycle::NumberValueSet<T>::Range::get_min() const { return _min; }
template <typename T>
requires Cycle::IsNumeric<T>
T Cycle::NumberValueSet<T>::Range::get_max() const { return _max; }
template <typename T>
requires Cycle::IsNumeric<T>
T Cycle::NumberValueSet<T>::Range::get_step() const { return _step; }

template <typename T>
requires Cycle::IsNumeric<T>
bool Cycle::NumberValueSet<T>::Range::contains(T value) const {
    if (value < _min || value > _max) return false;
    T rem = modulus(value - _min, _step);
    return compare_numerics(rem, (T)0);
}

template <typename T>
requires Cycle::IsNumeric<T>
bool Cycle::NumberValueSet<T>::Range::try_merge_with(const Range& other){
    if (_step != other.get_step()) return false;

    // If previous step
    if (_min - _step == other.get_max()){
        _merge_with(other);
        return true;
    }
    // If next step
    if (_max + _step == other.get_min()){
        _merge_with(other);
        return true;
    }
    // If out of range
    if (_min > other.get_max() || _max < other.get_min()){
        return false;
    }
    // Check if overlaps
    T smaller_min = std::min(_min, other.get_min());
    T bigger_min = std::max(_min, other.get_min());
    if (compare_numerics(modulus(bigger_min - smaller_min, _step), (T)0)){
        _merge_with(other);
        return true;
    }
    return false;
}

template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::Range::_merge_with(const Range& other){
    _min = std::min(_min, other.get_min());
    _max = std::max(_max, other.get_max());
    *this = Range(_min, _max, _step); // Reconstruct after modifications
}

// NumberValueSet<T>:

template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::add_value(T value){
    for (Range& range : _possible_ranges){
        Range new_range(value, value, range.get_step());
        if (range.try_merge_with(new_range)) return;
    }
    _possible_values.insert(value);
    _promote_values_to_range();
}

template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::add_range(const Range& value){
    _merge_new_range(value);
}

template <typename T>
requires Cycle::IsNumeric<T>
bool Cycle::NumberValueSet<T>::contains(T value) const {
    for (const T& possible_value : _possible_values){
        if (value == possible_value) return true;
    }
    for (const Range& possible_range : _possible_ranges){
        if (possible_range.contains(value)){
            return true;
        }
    }
    return false;
}

template <typename T>
requires Cycle::IsNumeric<T>
std::string Cycle::NumberValueSet<T>::print_value_set(uint indentation) const {
    std::ostringstream oss;
    for (uint i = 0; i < indentation; i++) oss << TAB_SPACE;
    oss << "{";
    for (auto it = _possible_values.begin(); it != _possible_values.end(); it++){
        oss << *it;
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
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::_promote_values_to_range(){
    if (_possible_values.size() < 3) return;

    std::vector<T> sorted(_possible_values.begin(), _possible_values.end());
    std::set<T> values_to_remove;

    for (std::size_t i = 0; i < sorted.size() - 2; i++){
        std::size_t initial_index = i;

        T first_value = sorted[i];
        T second_value = sorted[i + 1];
        T third_value = sorted[i + 2];

        T first_step = second_value - first_value;
        T second_step = third_value - second_value;

        if (first_step != second_step){
            continue;
        }
        T min = first_value;
        T max = third_value;
        T step = first_step;

        i += 3;
        while (i < sorted.size()){
            T new_step = sorted[i] - sorted[i - 1];
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
    for (T value : _possible_values){
        for (Range& range : _possible_ranges){
            Range new_range(value, value, range.get_step());
            if (range.try_merge_with(new_range)) return;
        }
    }
}

template <typename T>
requires Cycle::IsNumeric<T>
void Cycle::NumberValueSet<T>::_merge_new_range(const Range& new_range){
    for (Range& range : _possible_ranges){
        if (range.try_merge_with(new_range)) return;
    }
    _possible_ranges.push_back(new_range);
}
