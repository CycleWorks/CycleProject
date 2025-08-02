#pragma once

#include "Common/errors.hpp"
#include "system.hpp"
#include <filesystem>
#include <functional>
#include <string_view>
#include <memory>
#include <stdfloat>
#include <type_traits>
#include <cmath>

namespace fs = std::filesystem;
using uint = unsigned int;

// Removes annoying unnused 'fs' warning
[[maybe_unused]] constexpr bool _FS_USED = std::is_same_v<fs::path, fs::path>;

namespace Cycle {
    constexpr std::string_view TAB_SPACE = "    ";

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
    constexpr std::string_view type_name(){
        return typeid(T).name();
    }
    template <typename Comparison, typename InputType>
    bool ptr_cmp(const InputType* ptr){
        if constexpr (!std::is_polymorphic_v<Comparison>){
            throw InternalError("Pointer comparison failed: comparison type '{}' does not contain RTTI data", type_name<Comparison>());
        }
        if constexpr (!std::is_polymorphic_v<InputType>){
            throw InternalError("Pointer comparison failed: input type '{}' does not contain RTTI data", type_name<InputType>());
        }
        return dynamic_cast<const Comparison*>(ptr) != nullptr;
    }
    template <typename T, typename U, typename V>
    requires IsFloat<T> && IsFloat<U> && std::convertible_to<V, std::common_type_t<T, U>>
    bool compare_floats(T a, U b, V epsilon){
        using CommonType = std::common_type_t<T, U, V>;
        CommonType diff = std::abs(CommonType(a) - CommonType(b));
        CommonType max_val = std::max(std::abs(CommonType(a)), std::abs(CommonType(b)));
        return diff <= CommonType(epsilon) * max_val || diff <= CommonType(epsilon);
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    bool compare_numerics(T a, U b) {
        using CommonType = std::common_type_t<T, U>;

        if constexpr (IsInteger<T> && IsInteger<U>){
            return CommonType(a) == CommonType(b);
        } else if constexpr (IsFloat<T> || IsFloat<U>){
            CommonType epsilon = std::numeric_limits<CommonType>::epsilon();
            return compare_floats(CommonType(a), CommonType(b), epsilon * 2);
        } else {
            throw InternalError("Compare numeric function failed: unsupported type(s)");
        }
    }
    template <typename T, typename U>
    requires IsNumeric<T> && IsNumeric<U>
    long double modulus(T a, U b){
        using CommonType = std::common_type_t<T, U>;

        if (compare_numerics(CommonType(b), CommonType(0))){
            throw InternalError("Modulus function failed: division by zero");
        }
        if constexpr (IsInteger<T> && IsInteger<U>){
            return (long double)(CommonType(a) % CommonType(b));
        }
        else if constexpr (IsFloat<T> || IsFloat<U>){
            return std::fmod(CommonType(a), CommonType(b));
        } else {
            throw InternalError("Modulus function failed: unsupported type(s)");
        }
    }
    template <typename T>
    requires IsNumeric<T>
    T absolute(T num){
        if constexpr (IsSignedInteger<T>){
            return (num < 0) ? -num : num;
        } else if constexpr (IsUnsignedInteger<T>){
            return num;
        } else if constexpr (IsFloat<T>){
            return std::abs(num);
        } else {
            throw InternalError("Absolute function failed: unsupported type");
        }
    }

    struct AbstractClass {
        virtual ~AbstractClass(){}
    protected:
        virtual void polymorphism() const = 0;
    };

    template <typename Derived>
    struct BasicFactory {
        BasicFactory(const BasicFactory&) = delete;
        BasicFactory& operator=(const BasicFactory&) = delete;

        static std::unique_ptr<Derived> make_unique(Derived* new_class){
            return std::unique_ptr<Derived>(new_class);
        }
        static std::shared_ptr<Derived> make_shared(Derived* new_class){
            return std::shared_ptr<Derived>(new_class);
        }
    private:
        BasicFactory() = default;
        ~BasicFactory() = default;
    };
}
