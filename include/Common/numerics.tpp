#pragma once

#include "Common/numerics.hpp"
#include <limits>

namespace Cycle {
    // NumericWrapper<T>:

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
        return CommonType(other) < CommonType(*this);
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator<=(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(*this) < CommonType(other) || CommonType(*this) == CommonType(other);
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    bool NumericWrapper<T>::operator>=(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(*this) >= CommonType(other);
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator+(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) + CommonType(other.value());
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<std::common_type_t<T, U>> NumericWrapper<T>::operator-(const NumericWrapper<U>& other) const {
        using CommonType = std::common_type_t<T, U>;
        return CommonType(_number) - CommonType(other.value());
    }
    template <typename T>
    requires IsNumeric<T>
    NumericWrapper<T> NumericWrapper<T>::operator-() const {
        return -_number;
    }
    template <typename T>
    requires IsNumeric<T>
    template <typename U>
    requires IsNumeric<U>
    NumericWrapper<long double> NumericWrapper<T>::operator%(const NumericWrapper<U>& other) const {
        return _modulus(_number, other.value());
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

    // NumericWrapper helpers:

    template <typename T, typename U, typename V>
    requires IsFloat<T> && IsFloat<U> && std::convertible_to<V, std::common_type_t<T, U>>
    bool _compare_floats(T a, U b, V epsilon){
        using CommonType = std::common_type_t<T, U, V>;
        CommonType diff = std::abs(CommonType(a) - CommonType(b));
        CommonType max_val = std::max(std::abs(CommonType(a)), std::abs(CommonType(b)));
        return diff <= CommonType(epsilon) * max_val || diff <= CommonType(epsilon);
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    bool _compare_numerics(T a, U b){
        using CommonType = std::common_type_t<T, U>;

        if constexpr (IsInteger<T> && IsInteger<U>){
            return CommonType(a) == CommonType(b);
        } else if constexpr (IsFloat<T> || IsFloat<U>){
            CommonType epsilon = std::numeric_limits<CommonType>::epsilon();
            return _compare_floats(CommonType(a), CommonType(b), epsilon * 2);
        } else {
            throw InternalError("Compare numeric function failed: unsupported type(s)");
        }
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    long double _modulus(T a, U b){
        using CommonType = std::common_type_t<T, U>;

        if (_compare_numerics(CommonType(b), CommonType(0))){
            throw InternalError("Modulus function failed: division by zero");
        }
        if constexpr (IsInteger<T> && IsInteger<U>){
            return (long double)(a % b);
        }
        else if constexpr (IsFloat<T> || IsFloat<U>){
            return std::fmod(CommonType(a), CommonType(b));
        } else {
            throw InternalError("Modulus function failed: unsupported type(s)");
        }
    }

    // Other functions:

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
}
