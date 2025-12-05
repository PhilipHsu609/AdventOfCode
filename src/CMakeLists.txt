# Auto-discover year directories (any 4-digit directory)
file(GLOB YEAR_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "[0-9][0-9][0-9][0-9]")

foreach(YEAR_DIR ${YEAR_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${YEAR_DIR})
        message(STATUS "Found year: ${YEAR_DIR}")
        add_subdirectory(${YEAR_DIR})
    endif()
endforeach()