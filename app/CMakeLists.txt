# -------------------------------
# Create Cycle app
# -------------------------------

# Create final executable
add_executable(cycle main.cpp)

# Include include
target_include_directories(cycle PRIVATE "${CMAKE_SOURCE_DIR}/include/")

# Link with Cycle
target_link_libraries(cycle PRIVATE cycle_lib)
