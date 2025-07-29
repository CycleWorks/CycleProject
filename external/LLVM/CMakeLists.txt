# -------------------------------
# Integrate LLVM into Cycle
# -------------------------------

# Enable static linking
set(LLVM_USE_STATIC_LIBS ON)
set(LLVM_BUILD_LLVM_DYLIB OFF)

# Find LLVM
find_package(LLVM 18.1.8 EXACT REQUIRED CONFIG)

# Debug info
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Map needed components
llvm_map_components_to_libnames(LLVM_LIB_COMPONENT_LIBS
    Core
    Support
    IRReader
)

# Include LLVM headers
target_include_directories(cycle_lib PUBLIC ${LLVM_INCLUDE_DIRS})
target_compile_definitions(cycle_lib PUBLIC ${LLVM_DEFINITIONS})

# Link against LLVM
target_link_directories(cycle_lib PRIVATE ${LLVM_LIBRARY_DIRS})
target_link_libraries(cycle_lib PRIVATE ${LLVM_LIBRARIES})

# LLVM PCH
target_precompile_headers(cycle_lib PRIVATE "${CMAKE_SOURCE_DIR}/include/LLVM.hpp")
