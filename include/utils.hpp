#pragma once

#include <filesystem>
#include <memory>
#include <stdfloat>

namespace fs = std::filesystem;
using uint = unsigned int;

// Removes annoying ::fs:: warning
[[maybe_unused]] constexpr bool _fs_used = std::is_same_v<fs::path, fs::path>;

namespace Cycle {
    inline static const std::string TAB_SPACE = "    ";

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

    template <typename ChildClass>
    struct BasicFactory {
        static std::unique_ptr<ChildClass> make_unique(ChildClass* new_class){
            return std::unique_ptr<ChildClass>(new_class);
        }
        static std::shared_ptr<ChildClass> make_shared(ChildClass* new_class){
            return std::shared_ptr<ChildClass>(new_class);
        }
    };
}
