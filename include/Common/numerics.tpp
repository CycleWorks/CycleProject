#pragma once

#include "Common/errors.hpp"
#include "Common/math.hpp"
#include "Common/numerics.hpp"
#include <cmath>
#include <combaseapi.h>
#include <limits>
#include <sstream>
#include <utility>

namespace Cycle {
    // NumericWrapper<T>:

    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T>::NumericWrapper(T number): _number(number){
        if constexpr (IsFloat<T>){
            if (std::isinf(_number)){
                throw InternalError("NumericWrapper<T> failed: constructed class with infinite floating type");
            }
            if (std::isnan(_number)){
                throw InternalError("NumericWrapper<T> failed: constructed class with NaN floating type");
            }
        }
    }

    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>::NumericWrapper(const NumericWrapper<U>& other):
        NumericWrapper<T>((T)other.value())
    {}
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>::NumericWrapper(U number):
        NumericWrapper<T>((T)number)
    {}

    template <typename T>
    requires IsNumeric<T>
    std::string NumericWrapper<T>::to_string() const {
        std::ostringstream oss;
        if constexpr (sizeof(T) == 1) oss << (int)(T)_number;
        else oss << (T)_number;
        return oss.str();
    }

    template <typename T>
    requires IsNumeric<T>
    static NumericWrapper<T> smallest_unit(){
        if constexpr (IsInteger<T>) return 1;
        if constexpr (IsFloat<T>) return NumericWrapper<T>::epsilon();
        else throw InternalError("NumericWrapper<T> failed: Couldn't get smallest unit");
    }

    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator==(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return _compare_numerics(CommonType(_number), CommonType(other.value()));
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator<(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) < CommonType(other.value()) && !_compare_numerics(CommonType(_number), CommonType(other.value()));
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator>(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) > CommonType(other.value()) && !_compare_numerics(CommonType(_number), CommonType(other.value()));
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator<=(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) < CommonType(other.value()) || _compare_numerics(CommonType(_number), CommonType(other.value()));
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator>=(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) > CommonType(other.value()) || _compare_numerics(CommonType(_number), CommonType(other.value()));
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator+(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        if (_addition_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
            throw InternalError("NumericWrapper<T> failed: addition overflow");
        }
        return CommonType(_number) + CommonType(other.value());
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator-(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        if (_subtraction_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
            throw InternalError("NumericWrapper<T> failed: subtraction overflow");
        }
        return CommonType(_number) - CommonType(other.value());
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator*(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        if (_multiplication_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
            throw InternalError("NumericWrapper<T> failed: multiplication overflow");
        }
        return CommonType(_number) * CommonType(other.value());
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator/(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        if (other == 0){
            throw InternalError("NumericWrapper<T> failed: division by zero");
        }
        if (_division_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
            throw InternalError("NumericWrapper<T> failed: division overflow");
        }
        NumericWrapper<CommonType> result = CommonType(_number) / CommonType(other.value());
        if constexpr (IsFloat<T>){
            if (std::isnan(result.value())) throw InternalError("NumericWrapper<T> failed: division returned NaN");
        }
        return result;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator%(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        if (other == 0){
            throw InternalError("NumericWrapper<T> failed: modulo division by zero");
        }
        if (_division_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
            throw InternalError("NumericWrapper<T> failed: modulo division overflow");
        }
        NumericWrapper<CommonType> result = _modulus(_number, other.value());
        if constexpr (IsFloat<T>){
            if (std::isnan(result.value())) throw InternalError("NumericWrapper<T> failed: modulo division returned NaN");
        }
        return result;
    }
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> NumericWrapper<T>::operator-() const {
        if (_subtraction_will_overflow(NumericWrapper<T>(0), *this)){
            throw InternalError("NumericWrapper<T> failed: negation overflow");
        }
        return -_number;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator=(const NumericWrapper<U>& other){
        _number = other.value();
        return *this;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator+=(const NumericWrapper<U>& other){
        *this = *this + other;
        return *this;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator-=(const NumericWrapper<U>& other){
        *this = *this - other;
        return *this;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator*=(const NumericWrapper<U>& other){
        *this = *this * other;
        return *this;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator/=(const NumericWrapper<U>& other){
        *this = *this / other;
        return *this;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<T>& NumericWrapper<T>::operator%=(const NumericWrapper<U>& other){
        *this = *this % other;
        return *this;
    }

    // Helpers:

    template <typename T, typename U, typename V>
    requires IsFloat<T> && IsFloat<U>
    bool _compare_floats(T a, U b){
        using CommonType = std::common_type_t<T, U, V>;

        if (a == 0 && b == 0) return true;
        if (std::isinf(a) || std::isinf(b)) return a == b;
        if (std::isnan(a) || std::isnan(b)) return false;

        CommonType epsilon = std::numeric_limits<CommonType>::epsilon();
        CommonType denorm_min = std::numeric_limits<CommonType>::denorm_min();

        CommonType diff = std::abs(CommonType(a) - CommonType(b));
        CommonType max_val = std::max(std::abs(CommonType(a)), std::abs(CommonType(b)));

        CommonType rel_check = diff <= epsilon * max_val;
        CommonType abs_check = diff <= denorm_min;
        return rel_check || abs_check;
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    bool _compare_numerics(T a, U b){
        using CommonType = std::common_type_t<T, U>;

        if constexpr (IsInteger<T> && IsInteger<U>){
            return CommonType(a) == CommonType(b);
        } else if constexpr (IsFloat<T> || IsFloat<U>){
            return _compare_floats(a, b);
        } else {
            throw InternalError("Compare numeric function failed: unsupported type(s)");
        }
        std::unreachable();
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    long double _modulus(T a, U b){
        using CommonType = std::common_type_t<T, U>;

        if (_compare_numerics(CommonType(b), CommonType(0))){
            throw InternalError("Modulus function failed: division by zero");
        }
        if constexpr (IsInteger<T> && IsInteger<U>){
            return (long double)(((CommonType(a) % CommonType(b)) + CommonType(b)) % CommonType(b));
        }
        else if constexpr (IsFloat<T> || IsFloat<U>){
            return std::fmod((long double)a, (long double)b);
        } else {
            throw InternalError("Modulus function failed: unsupported type(s)");
        }
        std::unreachable();
    }
    template <typename T>
    requires IsNumeric<T>
    bool _addition_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if constexpr (IsInteger<T>){
            if (b > NumericWrapper<T>(0)){
                return a > NumericWrapper<T>(std::numeric_limits<T>::max() - b.value());
            } else if (b < NumericWrapper<T>(0)){
                return a < NumericWrapper<T>(std::numeric_limits<T>::min() - b.value());
            } else {
                return false;
            }
        } else if constexpr (IsFloat<T>){
            return std::isinf(a.value() + b.value());
        } else {
            throw InternalError("Addition overflow check function failed: unsupported type");
        }
        std::unreachable();
    }
    template <typename T>
    requires IsNumeric<T>
    bool _subtraction_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if constexpr (IsInteger<T>){
            if (b > NumericWrapper<T>(0)){
                return a < NumericWrapper<T>(std::numeric_limits<T>::min() + b.value());
            } else if (b < NumericWrapper<T>(0)){
                return a > NumericWrapper<T>(std::numeric_limits<T>::max() + b.value());
            } else {
                return false;
            }
        } else if constexpr (IsFloat<T>){
            return std::isinf(a.value() - b.value());
        } else {
            throw InternalError("Subtraction overflow check function failed: unsupported type");
        }
        std::unreachable();
    }
    template <typename T>
    requires IsNumeric<T>
    bool _multiplication_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if (absolute(a) < NumericWrapper<T>(1) || absolute(b) < NumericWrapper<T>(1)) return false;

        if constexpr (IsInteger<T>){
            if constexpr (IsSignedInteger<T>){
                if ((a == NumericWrapper<T>::min() && b == NumericWrapper<T>(-1)) || (b == NumericWrapper<T>::min() && a == NumericWrapper<T>(-1))){
                    return true;
                }
            }
            bool a_positive = a > NumericWrapper<T>(0);
            bool b_positive = b > NumericWrapper<T>(0);
            if (a_positive == b_positive){
                NumericWrapper<T> threshold = std::numeric_limits<T>::max() / absolute(b).value();
                return absolute(a) <= threshold;
            } else {
                NumericWrapper<T> threshold = std::numeric_limits<T>::min() / absolute(b.value());
                return absolute(a) >= threshold;
            }
        } else if constexpr (IsFloat<T>){
            return std::isinf(a.value() * b.value());
        } else {
            throw InternalError("Multiplication overflow check function failed: unsupported type");
        }
        std::unreachable();
    }
    template <typename T>
    requires IsNumeric<T>
    bool _division_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b){
        if (b == NumericWrapper<T>(0)) throw InternalError("Division overflow check function failed: division by zero");

        if constexpr (IsSignedInteger<T>){
            return a == NumericWrapper<T>::min() && b == NumericWrapper<T>(-1);
        } else if constexpr (IsInteger<T>){
            return false;
        } else if constexpr (IsFloat<T>){
            return std::isinf(a.value() / b.value());
        } else {
            throw InternalError("Division overflow check function failed: unsupported type");
        }
        std::unreachable();
    }
}
