# -------------------------------
# Compile Cycle sources
# -------------------------------

# Compile sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(cycle_lib STATIC ${SOURCES})

# Include include
target_include_directories(cycle_lib PUBLIC "${CMAKE_SOURCE_DIR}/include/")

# Define Cycle version macro
target_compile_definitions(cycle_lib PRIVATE CYCLE_VERSION="Alpha 1.0")
