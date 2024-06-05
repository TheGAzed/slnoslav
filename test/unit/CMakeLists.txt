add_executable(UNITTESTS main.c)

add_library(unit_tests "")

target_link_libraries(unit_tests
    PRIVATE
        greatest
        program
)

target_sources(unit_tests
    PRIVATE
        structures/board/test_board_istack.c
        structures/board/test_board_4x4s.c
        structures/board/test_board_6x4s.c
        structures/board/test_board_6x4u.c
        structures/board/test_board_10x10s.c
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
