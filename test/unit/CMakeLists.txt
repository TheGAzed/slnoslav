add_executable(UNITTESTS main.c)

add_library(unit_tests "")

target_link_libraries(unit_tests
    PRIVATE
        greatest
        program
)

target_sources(unit_tests
    PRIVATE
        structures/abstract/stack_test.c
)

target_include_directories(unit_tests
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UNITTESTS
    PRIVATE
        unit_tests
        greatest
        program
)

add_test(
    NAME UNIT_TEST
    COMMAND UNITTESTS
)
