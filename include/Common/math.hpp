#pragma once

#include "Common/numerics.hpp"

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
    struct ResultEGCD {
        NumericWrapper<T> gcd;
        NumericWrapper<T> x;
        NumericWrapper<T> y;
    };
    template <typename T>
    requires IsNumeric<T>
    struct ResultNumericVec2 {
        NumericWrapper<T> x;
        NumericWrapper<T> y;
    };

    template <typename T>
    requires IsNumeric<T>
    ResultEGCD<T> extended_gcd(NumericWrapper<T> a, NumericWrapper<T> b){
        if (b == 0) return {a, 1, 0};
        auto result = extended_gcd(b, a % b);
        return {result.gcd, result.y, result.x - (a / b) * result.y};
    }

    template<typename T>
    requires IsNumeric<T>
    std::optional<ResultNumericVec2<T>> solve_diophantine(NumericWrapper<T> a, NumericWrapper<T> b, NumericWrapper<T> c){
        auto result = extended_gcd(a, b);
        if (c % result.gcd != 0) return std::nullopt;
        NumericWrapper<T> x0 = result.x * (c / result.gcd);
        NumericWrapper<T> y0 = result.y * (c / result.gcd);
        return {x0, y0};
    }
}
