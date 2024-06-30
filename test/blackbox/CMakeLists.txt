add_executable(BLACKBOX main.c)

add_library(black_box "")

target_link_libraries(black_box
    PRIVATE
        PROGRAM_LIBRARY
        greatest
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

        suites/medium/medium_backtrack_forward_check.c
        suites/medium/medium_arc_consistency_backtrack_forward_check.c
)

target_include_directories(black_box
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(BLACKBOX
    PRIVATE
        black_box
        greatest
        PROGRAM_LIBRARY
)

add_test(NAME BLACKBOX_TEST COMMAND BLACKBOX WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
