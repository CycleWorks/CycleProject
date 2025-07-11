# Basic settings
cmake_minimum_required(VERSION 3.5)
project(CycleProject LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Enable compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Link-time optimisation
cmake_policy(SET CMP0069 NEW)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# Headers
include_directories("${CMAKE_SOURCE_DIR}/include/")

# Compile sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(cycle ${SOURCES})
target_compile_definitions(cycle PRIVATE CYCLE_VERSION="Alpha 1.0")
