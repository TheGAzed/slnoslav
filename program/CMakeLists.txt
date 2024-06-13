add_executable(${PROJECT_NAME} main.c)

add_library(PROGRAM_LIB "")

target_sources(PROGRAM_LIB
    PRIVATE
        source/algorithms/depth_first_search.c
        source/algorithms/backtrack.c
        source/algorithms/arc_consistency.c
        source/algorithms/forward_checking.c

        source/instance/argument.c
        source/instance/settings.c
        source/instance/statistics.c
        
        source/structures/concrete/board.c
        source/structures/concrete/state.c

        source/slnoslav.c
)

target_include_directories(PROGRAM_LIB PUBLIC include)

target_link_libraries(${PROJECT_NAME} PROGRAM_LIB)
