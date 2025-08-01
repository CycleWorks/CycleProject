#pragma once

#if __cplusplus <= 202002L
    #error Compiler must use C++23 or higher
#endif

#if defined(_WIN32)
    #define OS_WINDOWS
    #include <windows.h>
    #undef min
    #undef max
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC && !TARGET_OS_IPHONE
        #define OS_MACOS
        #include <pthread.h>
    #else
        #error Unsupported Apple platform
    #endif
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

#ifdef INTERFACE
    #undef INTERFACE
#endif
#ifdef interface
    #undef interface
#endif
#ifdef THIS
    #undef THIS
#endif

static_assert(CHAR_BIT == 8, "This platform does not use the standard 8-bit chararacter type. Compilation failed");
static_assert(sizeof(float) == 4, "This platform does not use 32-bit float types");
static_assert(sizeof(double) == 8, "This platform does not use 64-bit double types");
