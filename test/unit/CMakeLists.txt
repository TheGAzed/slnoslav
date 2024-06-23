add_executable(UNITTESTS main.c)

add_library(UNIT_TEST_LIB "")

target_link_libraries(UNIT_TEST_LIB
    PRIVATE
        GREATEST_LIB
        PROGRAM_LIBRARY
)

target_sources(UNIT_TEST_LIB
    PRIVATE
        structures/abstract/stack_test.c
)

target_include_directories(UNIT_TEST_LIB
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UNITTESTS
    PRIVATE
        UNIT_TEST_LIB
        GREATEST_LIB
        PROGRAM_LIBRARY
)

add_test(UNIT_TEST UNITTESTS)
