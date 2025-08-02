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
    constexpr bool ptr_cmp(const InputType* ptr){
        if constexpr (!std::is_polymorphic_v<Comparison>){
            throw InternalError("Pointer comparison failed: comparison type '{}' does not contain RTTI data", type_name<Comparison>());
        }
        if constexpr (!std::is_polymorphic_v<InputType>){
            throw InternalError("Pointer comparison failed: input type '{}' does not contain RTTI data", type_name<InputType>());
        }
        return dynamic_cast<const Comparison*>(ptr) != nullptr;
    }
    template <typename T>
    requires IsFloat<T>
    constexpr bool compare_floats(T a, T b, T epsilon){
        return std::fabs(a - b) <= epsilon;
    }
    template <typename T>
    requires IsNumeric<T>
    constexpr bool compare_numerics(T a, T b){
        if constexpr (IsInteger<T>){
            return a == b;
        } else if constexpr (IsFloat<T>){
            T min_epsilon = std::numeric_limits<T>::epsilon();
            return compare_floats(a, b, min_epsilon * 2);
        } else {
            throw InternalError("Compare numeric function failed: unsupported type");
        }
    }
    template <typename T>
    requires IsNumeric<T>
    constexpr long double modulus(T a, T b){
        if constexpr (compare_numerics(b, 0)){
            throw InternalError("Modulus function failed: division by zero");
        }
        if constexpr (IsInteger<T>){
            return (long double)(a % b);
        } else if constexpr (IsFloat<T>){
            return std::fmod(a, b);
        } else {
            throw InternalError("Modulus function failed: unsupported type");
        }
    }
    template <typename T>
    requires IsNumeric<T>
    T absolute(T num){
        if constexpr (IsSignedInteger<T>){
            return (num < 0)? -num : num;
        } else if constexpr (IsUnsignedInteger<T>){
            return num;
        } else if constexpr (IsFloat<T>){
            T min_epsilon = std::numeric_limits<T>::epsilon();
            return (num < min_epsilon * 2)? -num : num;
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
