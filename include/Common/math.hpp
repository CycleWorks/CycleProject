#pragma once

#include "Common/errors.hpp"
#include "Common/numerics.hpp"
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
    bool addition_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b) {
        if (b > NumericWrapper<T>(0) && a > NumericWrapper<T>::max() - b) return true;
        if (b < NumericWrapper<T>(0) && a < NumericWrapper<T>::min() - b) return true;
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    bool subtraction_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b) {
        if (b > NumericWrapper<T>(0) && a < NumericWrapper<T>::min() + b) return true;
        if (b < NumericWrapper<T>(0) && a > NumericWrapper<T>::max() + b) return true;
        return false;
    }

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<long double> ceil_division(NumericWrapper<T> a, NumericWrapper<T> b){
        NumericWrapper<long double> div = a / b;
        NumericWrapper<long double> rem = a % b;

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
    NumericWrapper<long double> floor_division(NumericWrapper<T> a, NumericWrapper<T> b){
        NumericWrapper<long double> div = a / b;
        NumericWrapper<long double> rem = a % b;

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
