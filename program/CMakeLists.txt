add_executable(${PROJECT_NAME} main.c)

add_library(program "")

target_sources(program
    PRIVATE
        source/algorithms/depth_first_search.c
        source/algorithms/backtrack.c
        source/algorithms/arc_consistency.c
        source/algorithms/forward_checking.c

        source/instance/argument.c
        source/instance/settings.c
        
        source/structures/concrete/board.c
        source/structures/concrete/state.c

        source/slnoslav.c
)

target_include_directories(program PUBLIC include)

target_link_libraries(${PROJECT_NAME} program)
