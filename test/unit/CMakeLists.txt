add_executable(UNITTESTS main.c)

target_sources(UNITTESTS
    PRIVATE
        structures/abstract/stack_test.c
)

target_include_directories(UNITTESTS
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UNITTESTS
    PRIVATE
        greatest
        program
)

add_test(NAME UNIT_TEST COMMAND UNITTESTS WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
