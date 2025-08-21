#pragma once

#include "Common/errors.hpp"
#include "Common/math.hpp"
#include "Common/numerics.hpp"
#include <cmath>
#include <limits>
#include <sstream>

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
    std::string NumericWrapper<T>::to_string() const {
        std::ostringstream oss;
        if constexpr (sizeof(T) == 1) oss << (int)(T)_number;
        else oss << (T)_number;
        return oss.str();
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
        if (addition_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
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
        if (subtraction_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
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
        if (multiplication_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
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
        if (other.value() == 0){
            throw InternalError("NumericWrapper<T> failed: division by zero");
        }
        if (division_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
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
        if (other.value() == 0){
            throw InternalError("NumericWrapper<T> failed: modulo division by zero");
        }
        if (division_will_overflow(NumericWrapper<CommonType>(_number), NumericWrapper<CommonType>(other.value()))){
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
        if (subtraction_will_overflow(NumericWrapper<T>(0), *this)){
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
            return (long double)(((CommonType(a) % CommonType(b)) + CommonType(b)) % CommonType(b));
        }
        else if constexpr (IsFloat<T> || IsFloat<U>){
            return std::fmod((long double)a, (long double)b);
        } else {
            throw InternalError("Modulus function failed: unsupported type(s)");
        }
    }
}
