#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
    #include <windows.h>
    #undef min
    #undef max
    #undef TRUE
    #undef FALSE
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MACOS
    #include <pthread.h>
#elif defined(__linux__)
    #define OS_LINUX
    #include <pthread.h>
    #include <unistd.h>
#else
    #error Compiling with unsupported operating system
#endif

#if defined(OS_MACOS) || defined(OS_LINUX)
    #define OS_SUPPORTS_ANSI_ESCAPES
#endif

#include <filesystem>
#include <memory>
#include <stdfloat>

#ifdef INTERFACE
    #undef INTERFACE
#endif
#ifdef interface
    #undef interface
#endif
#ifdef THIS
    #undef THIS
#endif

#if CHAR_BIT != 8
    #error This platform does not use the standard 8-bit chararacter type. Compilation failed
#endif

static_assert(sizeof(float) == 4, "This platform does not use 32-bit float types");
static_assert(sizeof(double) == 8, "This platform does not use 64-bit double types");

namespace fs = std::filesystem;
using uint = unsigned int;

// Removes annoying warning
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
}
