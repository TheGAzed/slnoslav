add_executable(BLACKBOX main.c)

target_sources(BLACKBOX
    PRIVATE
        suites/easy/easy_unset.c
        suites/easy/easy_fch.c
        suites/easy/easy_bt.c
        suites/easy/easy_bt_fch.c
        suites/easy/easy_ac.c
        suites/easy/easy_ac_fch.c
        suites/easy/easy_ac_bt.c
        suites/easy/easy_ac_bt_fch.c

        suites/medium/medium_backtrack_forward_check.c
        suites/medium/medium_arc_consistency_backtrack_forward_check.c
)

target_include_directories(BLACKBOX
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(BLACKBOX
    PRIVATE
        greatest
        program
)

add_test(
        NAME BLACKBOX_TEST
        COMMAND BLACKBOX
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
