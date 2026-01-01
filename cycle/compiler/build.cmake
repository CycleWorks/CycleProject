# -------------------------------
# Build Cycle compiler
# -------------------------------

# Compile sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_library(cycle_compiler STATIC ${SOURCES})

# Include include
target_include_directories(cycle_compiler PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
