add_executable(BLACKBOX main.c)

add_library(black_box "")

target_link_libraries(black_box
    PRIVATE
        greatest
        program
)

target_sources(black_box
    PRIVATE
        suites/easy/easy_unset.c
        suites/easy/easy_forward_check.c
        suites/easy/easy_backtrack.c
        suites/easy/easy_backtrack_forward_check.c
        suites/easy/easy_arc_consistency.c
        suites/easy/easy_arc_consistency_forward_check.c
        suites/easy/easy_arc_consistency_backtrack.c
        suites/easy/easy_arc_consistency_backtrack_forward_check.c
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
