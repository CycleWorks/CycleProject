#pragma once

#include "Common/errors.hpp"
#include "Common/numerics.hpp"
#include <cmath>
#include <limits>
#include <optional>
#include <type_traits>

namespace Cycle {
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> absolute(NumericWrapper<T> num){
        if constexpr (IsUnsignedInteger<T>){
            return num;
        } else if constexpr (IsInteger<T> || IsFloat<T>){
            return std::abs(num.value());
        } else {
            throw InternalError("Absolute function failed: unsupported type");
        }
    }
    template <typename T>
    requires IsNumeric<T>
    bool addition_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        return _addition_will_overflow(a, b);
    }
    template <typename T>
    requires IsNumeric<T>
    bool subtraction_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        return _subtraction_will_overflow(a, b);
    }
    template <typename T>
    requires IsNumeric<T>
    bool multiplication_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        return _multiplication_will_overflow(a, b);
    }
    template <typename T>
    requires IsNumeric<T>
    bool division_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        return _division_will_overflow(a, b);
    }
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> ceil_division(NumericWrapper<T> a, NumericWrapper<T> b){
        if (b == NumericWrapper<T>(0)) throw InternalError("Ceil division function failed: division by zero");

        if constexpr (IsInteger<T>){
            NumericWrapper<T> q = a / b;
            NumericWrapper<T> r = a % b;
            if (r == NumericWrapper<T>(0)) return q;
            return ((a > NumericWrapper<T>(0)) == (b > NumericWrapper<T>(0))) ? q + NumericWrapper<T>(1) : q;
        } else if constexpr (IsFloat<T>){
            return std::ceil((a / b).value());
        }
    }

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> floor_division(NumericWrapper<T> a, NumericWrapper<T> b){
        if (b == NumericWrapper<T>(0)) throw InternalError("Floor division function failed: division by zero");
        if constexpr (IsInteger<T>) {
            NumericWrapper<T> q = a / b;
            NumericWrapper<T> r = a % b;
            if (r == NumericWrapper<T>(0)) return q;
            return ((a > NumericWrapper<T>(0)) != (b > NumericWrapper<T>(0))) ? q - NumericWrapper<T>(1) : q;
        } else if constexpr (IsFloat<T>) {
            return std::floor((a / b).value());
        }
    }
}
