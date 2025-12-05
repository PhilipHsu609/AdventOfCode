# Reusable function to build all days for a given year
function(aoc_add_year_solutions YEAR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/cpp)

    # Auto-discover all Day*/solve.cpp files with CONFIGURE_DEPENDS
    # This makes CMake track when new days are added
    file(GLOB SOLUTIONS CONFIGURE_DEPENDS Day*/solve.cpp)

    foreach(SOLUTION ${SOLUTIONS})
        # Extract day number (handles Day1, Day01, Day23, etc.)
        string(REGEX MATCH "Day0*([0-9]+)" DAY_MATCH ${SOLUTION})
        set(DAY_NUM ${CMAKE_MATCH_1})

        # Create target name: YEAR_DayXX
        set(TARGET_NAME "${YEAR}_Day${DAY_NUM}")

        message(STATUS "  Adding solution: ${TARGET_NAME}")

        add_executable(${TARGET_NAME} ${SOLUTION})
        target_link_libraries(${TARGET_NAME} PRIVATE
            compile_flags_interface
            fmt::fmt
            aoc_utils
        )
        target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    endforeach()
endfunction()
