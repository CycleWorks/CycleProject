# -------------------------------
# Compilation settings
# -------------------------------

# C++ standard
set(ENFORCED_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD ${ENFORCED_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Enable compile commands (for IDE linting)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Link-time optimisation (LTO)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT output)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    message(STATUS "LTO not supported by compiler, disabling: ${output}")
endif()

# -------------------------------
# Include Cycle subfolders
# -------------------------------

include(cycle/utils/build.cmake)
include(cycle/compiler/build.cmake)
include(cycle/app/build.cmake)
