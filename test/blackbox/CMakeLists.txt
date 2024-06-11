add_executable(BLACKBOX main.c)

add_library(black_box "")

target_link_libraries(black_box
    PRIVATE
        greatest
        program
)

target_sources(black_box
    PRIVATE
        suites/easy_puzzles.c
)

target_include_directories(black_box
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(BLACKBOX
    PRIVATE
        black_box
        greatest
        program
)
