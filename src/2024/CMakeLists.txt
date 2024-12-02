set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/cpp)

set(SOLUTIONS
    Day1/solve.cpp
)

foreach(SOLUTION ${SOLUTIONS})
    get_filename_component(DAY ${SOLUTION} DIRECTORY)
    string(PREPEND DAY "2024_")
    add_executable(${DAY} ${SOLUTION})
    target_link_libraries(${DAY} PRIVATE compile_flags_interface fmt::fmt)
    target_include_directories(${DAY} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach()