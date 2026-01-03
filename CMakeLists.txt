# -------------------------------
# CMake settings
# -------------------------------

cmake_minimum_required(VERSION 4.0)
project(CycleProject LANGUAGES CXX)

# -------------------------------
# Include project subfolders
# -------------------------------

include(deps/build.cmake)
include(cycle/build.cmake)
