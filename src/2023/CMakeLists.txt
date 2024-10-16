set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/cpp)

set(SOLUTIONS
    Day1/solve.cpp
    Day2/solve.cpp
    Day3/solve.cpp
    Day4/solve.cpp
    Day5/solve.cpp
    Day6/solve.cpp
    Day7/solve.cpp
    Day8/solve.cpp
    Day9/solve.cpp
    Day10/solve.cpp
    Day11/solve.cpp
    Day12/solve.cpp
    Day13/solve.cpp
)

foreach(SOLUTION ${SOLUTIONS})
    get_filename_component(DAY ${SOLUTION} DIRECTORY)
    string(PREPEND DAY "2023_")
    add_executable(${DAY} ${SOLUTION})
    target_link_libraries(${DAY} PRIVATE compile_flags_interface fmt::fmt)
    target_include_directories(${DAY} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach()