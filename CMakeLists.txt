# -------------------------------
# CMake settings
# -------------------------------

cmake_minimum_required(VERSION 3.16)
project(CycleProject LANGUAGES CXX VERSION 1.0.0)

# -------------------------------
# Include project subfolders
# -------------------------------

include(deps/build.cmake)
include(cycle/build.cmake)
