cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(
    AdventOfCode
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# vcpkg dependencies
find_package(fmt CONFIG REQUIRED)

# Define an interface library
add_library(compile_flags_interface INTERFACE)

# Set compiler flags for the interface library
target_compile_options(compile_flags_interface INTERFACE
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Woverloaded-virtual

    # -Wconversion
    -Wmisleading-indentation
    -Wnull-dereference
    -pedantic
    $<$<CONFIG:DEBUG>:-g>
    $<$<CONFIG:RELEASE>:-O3>
)

# Build utils library
add_library(aoc_utils util/utils.cpp)
target_include_directories(aoc_utils PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(aoc_utils PUBLIC compile_flags_interface)

add_subdirectory(src)