add_executable(TESTS main.c)

add_library(test "")

target_link_libraries(test
    PRIVATE
        greatest
        program
)

target_sources(test
    PRIVATE
        unit/structures/board/test_board_istack.c
        unit/structures/board/test_board_4x4s.c
        unit/structures/board/test_board_6x4s.c
        unit/structures/board/test_board_6x4u.c
        unit/structures/board/test_board_10x10s.c
)

target_include_directories(test
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(TESTS
    PRIVATE
        test
        greatest
        program
)
