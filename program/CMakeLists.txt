add_executable(${PROJECT_NAME} main.c)

add_library(program "")

target_sources(program
    PRIVATE
        source/algorithms/depth_first_search.c
        source/algorithms/backtrack.c
        source/algorithms/forward_check.c

        source/structures/argument.c
        source/structures/board.c
        source/structures/state.c

        source/slnoslav.c
)

target_include_directories(program PUBLIC include)

target_link_libraries(${PROJECT_NAME} program)
