set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/cpp)

file(GLOB SOLUTIONS Day*/solve.cpp)

foreach(SOLUTION ${SOLUTIONS})
    string(REGEX MATCH "Day[0-9]+" DAY ${SOLUTION})
    string(PREPEND DAY "2023_")
    add_executable(${DAY} ${SOLUTION})
    target_link_libraries(${DAY} PRIVATE compile_flags_interface fmt::fmt)
    target_include_directories(${DAY} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endforeach()