#pragma once

#include <limits>
#include <type_traits>
#include <format>

namespace Cycle {
    template <typename T>
    concept IsInteger = std::is_integral_v<T> && !std::is_same_v<T, bool>;
    template <typename T>
    concept IsSignedInteger = IsInteger<T> && std::is_signed_v<T>;
    template <typename T>
    concept IsUnsignedInteger = IsInteger<T> && std::is_unsigned_v<T>;
    template <typename T>
    concept IsFloat = std::is_floating_point_v<T>;
    template <typename T>
    concept IsNumeric = IsInteger<T> || IsFloat<T>;

    template <typename T>
    requires IsNumeric<T>
    struct NumericWrapper {
        NumericWrapper() = delete;
        NumericWrapper(const NumericWrapper<T>&) = default;
        NumericWrapper(T number);

        template <typename U>
        requires IsNumeric<U>
        explicit NumericWrapper(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        explicit NumericWrapper(U number);

        std::string to_string() const;

        static NumericWrapper<T> min(){ return std::numeric_limits<T>::min(); }
        static NumericWrapper<T> max(){ return std::numeric_limits<T>::max(); }
        static NumericWrapper<T> elipson(){ return std::numeric_limits<T>::epsilon(); }

        static NumericWrapper<T> smallest_unit();

        explicit operator T(){ return _number; }
        explicit operator const T() const { return _number; }
        T value(){ return _number; }
        const T value() const { return _number; }

        template <typename U>
        requires IsNumeric<U>
        bool operator==(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        bool operator<(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        bool operator>(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        bool operator<=(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        bool operator>=(const NumericWrapper<U>& other) const;

        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<std::common_type_t<T, U>> operator+(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<std::common_type_t<T, U>> operator-(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<std::common_type_t<T, U>> operator*(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<std::common_type_t<T, U>> operator/(const NumericWrapper<U>& other) const;
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<std::common_type_t<T, U>> operator%(const NumericWrapper<U>& other) const;

        NumericWrapper<T> operator-() const;

        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator=(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator+=(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator-=(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator*=(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator/=(const NumericWrapper<U>& other);
        template <typename U>
        requires IsNumeric<U>
        NumericWrapper<T>& operator%=(const NumericWrapper<U>& other);
    private:
        T _number;
    };

    // Helpers:

    template <typename T, typename U, typename V>
    requires IsFloat<T> && IsFloat<U> && std::convertible_to<V, std::common_type_t<T, U>>
    bool _compare_floats(T a, U b, V epsilon);

    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    bool _compare_numerics(T a, U b);

    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    long double _modulus(T a, U b);

    template <typename T>
    requires IsNumeric<T>
    bool _addition_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b);
    template <typename T>
    requires IsNumeric<T>
    bool _subtraction_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b);
    template <typename T>
    requires IsNumeric<T>
    bool _multiplication_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b);
    template <typename T>
    requires IsNumeric<T>
    bool _division_will_overflow(NumericWrapper<T> a, NumericWrapper<T> b);
}

#include "numerics.tpp"

namespace std {
    template <typename T>
    struct std::formatter<Cycle::NumericWrapper<T>> : std::formatter<std::string> {
        auto format(Cycle::NumericWrapper<T> numeric, format_context& ctx) const {
            return std::formatter<std::string>::format(
                std::format("{}", numeric.value()), ctx
            );
        }
    };
}
