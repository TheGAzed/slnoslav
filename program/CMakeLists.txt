find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

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
        source/instance/expect.c

        source/structures/concrete/board.c
        source/structures/concrete/state.c
        source/slnoslav.c
)

target_include_directories(program PUBLIC include)

target_link_libraries(program PRIVATE glfw GLEW::GLEW OpenGL::GL m nuklear)
target_link_libraries(${PROJECT_NAME} PRIVATE program)

target_compile_definitions(${PROJECT_NAME} PRIVATE ERROR_LOG_FILE_PATH=\"${CMAKE_BINARY_DIR}/slnoslav-debug.txt\")
