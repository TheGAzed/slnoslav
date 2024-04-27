add_executable(${PROJECT_NAME} main.c)

add_library(prog "")

target_sources(prog
    PRIVATE
        src/alg/depth_first_search.c
        src/alg/backtrack.c
        src/alg/forward_check.c

        src/ds/board.c
        src/ds/state.c

        src/slnoslav.c
    PUBLIC
        inc/alg/depth_first_search.h
        inc/alg/backtrack.h
        inc/alg/forward_check.h

        inc/ds/board.h
        inc/ds/state.h

        inc/slnoslav.h
)

target_include_directories(prog PUBLIC inc)

target_link_libraries(${PROJECT_NAME} prog)
