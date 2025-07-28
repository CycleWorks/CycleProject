# -------------------------------
# Test integration with Catch2
# -------------------------------

# Compile test sources
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(run_tests ${TEST_SOURCES})

# Link with Cycle
target_link_libraries(run_tests PRIVATE cycle_lib Catch2::Catch2)

# Enable testing
enable_testing()
catch_discover_tests(run_tests)

# Run tests
add_custom_command(TARGET run_tests
    POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running tests after build"
)
