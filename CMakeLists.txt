cmake_minimum_required(VERSION 3.10)  # Minimum CMake version

# Project definition
project(MCacheProject VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include folder for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/MCache.cpp
    src/cli.cpp
)

# Add executable for main program
add_executable(MCacheApp ${SOURCES})

# ---- Optional: Test executable ----
enable_testing()  # Enable CTest

# Add test source
add_executable(MCacheTests tests/test_cache.cpp src/MCache.cpp)

# Link libraries if needed (none for now)
# target_link_libraries(MCacheTests ...)

add_test(NAME CacheTests COMMAND MCacheTests)
