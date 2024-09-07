find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.c)

add_library(program
        source/gui/graphics.c
        source/gui/input.c
        source/gui/draw.c

        source/gui/interface/grid.c
        source/gui/interface/solver.c
        source/gui/interface/biggest_rectangle.c
        source/gui/interface/interface.c

        source/algorithms/depth_first_search.c
        source/algorithms/backtrack.c
        source/algorithms/arc_consistency.c
        source/algorithms/reduce.c
        source/algorithms/forward_checking.c

        source/instance/argument.c
        source/instance/settings.c
        source/instance/statistics.c

        source/structures/concrete/board.c
        source/structures/concrete/state.c
        source/slnoslav.c
        include/style.h
)

target_include_directories(program PUBLIC include assets)

target_compile_definitions(program PRIVATE ERROR_LOG_FILE_PATH=\"${CMAKE_BINARY_DIR}/debug_log.txt\")

target_link_libraries(program
        PRIVATE glfw
        PRIVATE libglew_static
        PRIVATE glm
        nuklear
)

target_link_libraries(${PROJECT_NAME} PRIVATE program)
