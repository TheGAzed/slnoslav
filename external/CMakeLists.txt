add_library(greatest INTERFACE "")
target_include_directories(greatest INTERFACE greatest)

add_library(nuklear INTERFACE "")
target_include_directories(nuklear INTERFACE nuklear nuklear/demo/glfw_opengl3)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

add_definitions(-DGLEW_STATIC)
add_subdirectory(glew EXCLUDE_FROM_ALL)

add_subdirectory(glm)