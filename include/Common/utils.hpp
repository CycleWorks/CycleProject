#pragma once

#include "Common/errors.hpp"
#include "system.hpp"
#include <filesystem>

namespace fs = std::filesystem;
using uint = unsigned int;

// Removes annoying unnused 'fs' warning
[[maybe_unused]] constexpr bool _FS_USED = std::is_same_v<fs::path, fs::path>;

namespace Cycle {
    constexpr std::string_view TAB_SPACE = "    ";

    template <typename T>
    constexpr std::string_view type_name(){
        return typeid(T).name();
    }
    template <typename Comparison, typename InputType>
    requires std::is_polymorphic_v<Comparison> && std::is_polymorphic_v<InputType>
    bool ptr_cmp(const InputType* ptr){
        return dynamic_cast<const Comparison*>(ptr) != nullptr;
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
