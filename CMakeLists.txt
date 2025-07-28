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
# Include project subfolders
# -------------------------------

add_subdirectory(src)
add_subdirectory(LLVM)
add_subdirectory(app)
add_subdirectory(Catch2)
add_subdirectory(tests)
