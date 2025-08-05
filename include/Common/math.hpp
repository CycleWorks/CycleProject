#pragma once

#include "Common/errors.hpp"
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
    NumericWrapper<T> standard_gcd(NumericWrapper<T> a, NumericWrapper<T> b){
        a = absolute(a);
        b = absolute(b);

        if (a == 0) return b;
        if (b == 0) return a;

        while (b > NumericWrapper<T>::epsilon()){
            NumericWrapper<T> temp = a % b;
            a = b;
            b = temp;
        }
        return a;
    }

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
