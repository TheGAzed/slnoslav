add_library(greatest INTERFACE "")

target_include_directories(greatest INTERFACE greatest)

add_library(nuklear INTERFACE "")

target_include_directories(nuklear INTERFACE nuklear nuklear/demo/glfw_opengl3)

find_package(OpenGL REQUIRED)

if (!WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor wayland-egl xkbcommon)
	find_package(X11 REQUIRED)
	find_package(Doxygen)
endif()

add_subdirectory(glew)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
