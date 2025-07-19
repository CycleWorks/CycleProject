#pragma once

#if __cplusplus <= 202002L
    #error Compiler must use C++23 or higher
#endif

#include <filesystem>
#include <string_view>
#include <memory>
#include <stdfloat>

namespace fs = std::filesystem;
using uint = unsigned int;

// Removes annoying ::fs:: warning
[[maybe_unused]] constexpr bool _FS_USED = std::is_same_v<fs::path, fs::path>;

namespace Cycle {
    constexpr std::string_view TAB_SPACE = "    ";

    template <typename Comparison, typename InputType>
    constexpr bool ptr_cmp(const InputType* ptr){
        if constexpr (!std::is_polymorphic<Comparison>::value){
            throw InternalError("Couldn't compare pointers because comparison type '{}' does not contain RTTI data", type_name<Comparison>());
        }
        if constexpr (!std::is_polymorphic<InputType>::value){
            throw InternalError("Couldn't compare pointers because input type '{}' does not contain RTTI data", type_name<InputType>());
        }
        return dynamic_cast<const Comparison*>(ptr) != nullptr;
    }

    struct AbstractClass {
        virtual ~AbstractClass(){}
    protected:
        virtual void override_for_constructibility() const = 0;
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

    template <typename Derived>
    struct BasicSingleton {
        BasicSingleton(const BasicSingleton&) = delete;
        BasicSingleton& operator=(const BasicSingleton&) = delete;

        static Derived* get(){
            static Derived thiz;
            return &thiz;
        }
    private:
        BasicSingleton() = default;
        ~BasicSingleton() = default;
    };
}
