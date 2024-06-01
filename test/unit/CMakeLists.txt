add_executable(UNITTESTS main.c)

add_library(unittest "")

target_link_libraries(unittest
    PRIVATE
        greatest
        program
)

target_sources(unittest
    PRIVATE
        structures/board/test_board_istack.c
        structures/board/test_board_4x4s.c
        structures/board/test_board_6x4s.c
        structures/board/test_board_6x4u.c
        structures/board/test_board_10x10s.c
)

target_include_directories(unittest
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UNITTESTS
    PRIVATE
        unittest
        greatest
        program
)
