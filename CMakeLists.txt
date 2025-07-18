# -------------------------------
# Basic settings
# -------------------------------

cmake_minimum_required(VERSION 3.16)
project(CycleProject LANGUAGES CXX VERSION 1.0.0)

# C++ standard
set(ENFORCED_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD ${ENFORCED_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Enable compile commands (for tooling)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Link-time optimisation (LTO)
cmake_policy(SET CMP0069 NEW)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# Make '__cplusplus' macro compatible with other compilers
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# -------------------------------
# Compile and link Cycle
# -------------------------------

# Compile sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(cycle ${SOURCES})

# Include include
target_include_directories(cycle PRIVATE "${CMAKE_SOURCE_DIR}/include/")

# Define Cycle version macro
target_compile_definitions(cycle PRIVATE CYCLE_VERSION="Alpha 1.0")

# -------------------------------
# LLVM Integration
# -------------------------------

# Enable static linking
set(LLVM_USE_STATIC_LIBS ON)

# Find LLVM
find_package(LLVM 18.1.8 EXACT REQUIRED CONFIG)

# Debug info
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM headers
target_include_directories(cycle SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

# Map components to dynamic libraries
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    IRReader
)

# Link dynamically against LLVM
target_link_libraries(cycle PRIVATE ${LLVM_LIBS})

# Define LLVM macros
target_compile_definitions(cycle PRIVATE ${LLVM_DEFINITIONS})

# LLVM PCH
target_precompile_headers(cycle PRIVATE "${CMAKE_SOURCE_DIR}/include/LLVM.hpp")
