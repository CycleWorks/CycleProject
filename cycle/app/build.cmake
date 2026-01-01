# -------------------------------
# Build final executable
# -------------------------------

# Create final executable
add_executable(cycle "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

# Link with compiler
target_link_libraries(cycle PRIVATE cycle_compiler)
