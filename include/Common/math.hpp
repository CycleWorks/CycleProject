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
        if constexpr (IsSignedInteger<T>){
            return (num < NumericWrapper<T>(0)) ? -num : num;
        } else if constexpr (IsUnsignedInteger<T>){
            return num;
        } else if constexpr (IsFloat<T>){
            return std::abs(num.value());
        } else {
            throw InternalError("Absolute function failed: unsupported type");
        }
    }

    template <typename T>
    requires IsNumeric<T>
    bool addition_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if (b > NumericWrapper<T>(0) && a > NumericWrapper<T>(std::numeric_limits<T>::max() - b.value())) return true;
        if (b < NumericWrapper<T>(0) && a < NumericWrapper<T>(std::numeric_limits<T>::min() - b.value())) return true;
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    bool subtraction_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b) {
        if (b > NumericWrapper<T>(0) && a < NumericWrapper<T>(std::numeric_limits<T>::min() + b.value())) return true;
        if (b < NumericWrapper<T>(0) && a > NumericWrapper<T>(std::numeric_limits<T>::max() + b.value())) return true;
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    bool multiplication_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if (a == NumericWrapper<T>(0) || b == NumericWrapper<T>(0)) return false;

        if constexpr (IsSignedInteger<T>) {
            if (a == NumericWrapper<T>::min() && b == NumericWrapper<T>(-1)) return true;
            if (b == NumericWrapper<T>::min() && a == NumericWrapper<T>(-1)) return true;
        }
        if (a > NumericWrapper<T>(0)){
            if (b > NumericWrapper<T>(0)){
                return a > NumericWrapper<T>::max() / b;
            } else {
                return b < NumericWrapper<T>::min() / a;
            }
        } else {
            if (b > NumericWrapper<T>(0)){
                return a < NumericWrapper<T>::min() / b;
            } else {
                return a < NumericWrapper<T>::max() / b;
            }
        }
    }

    template <typename T>
    requires IsNumeric<T>
    bool division_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if (a == NumericWrapper<T>(0) || b == NumericWrapper<T>(0)) return false;

        if constexpr (IsSignedInteger<T>){
            if (a == NumericWrapper<T>::min() && b == NumericWrapper<T>(-1)) return true;
        }
        else if constexpr (IsFloat<T>){
            T result = (T)a / (T)b;
            return std::isinf(result);
        }
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> ceil_division(NumericWrapper<T> a, NumericWrapper<T> b){
        NumericWrapper<T> div = a / b;
        NumericWrapper<T> rem = a % b;

        if (rem == NumericWrapper<T>(0)){
            return div;
        }
        if ((a > NumericWrapper<T>(0) && b > NumericWrapper<T>(0)) || (a < NumericWrapper<T>(0) && b < NumericWrapper<T>(0))){
            return div + NumericWrapper<T>(1);
        } else {
            return div;
        }
    }

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> floor_division(NumericWrapper<T> a, NumericWrapper<T> b){
        NumericWrapper<T> div = a / b;
        NumericWrapper<T> rem = a % b;

        if (rem == NumericWrapper<T>(0)){
            return div;
        }
        if ((a > NumericWrapper<T>(0) && b < NumericWrapper<T>(0)) || (a < NumericWrapper<T>(0) && b > NumericWrapper<T>(0))){
            return div + NumericWrapper<T>(1);
        } else {
            return div;
        }
    }
}
