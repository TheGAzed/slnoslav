add_executable(BLACKBOX main.c)

add_library(BLACK_BOX_LIB "")

target_link_libraries(BLACK_BOX_LIB
    PRIVATE
        GREATEST_LIB
        PROGRAM_LIBRARY
)

target_sources(BLACK_BOX_LIB
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

target_include_directories(BLACK_BOX_LIB
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(BLACKBOX
    PRIVATE
        BLACK_BOX_LIB
        GREATEST_LIB
        PROGRAM_LIBRARY
)

add_test(NAME BLACKBOX_TEST COMMAND BLACKBOX WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
