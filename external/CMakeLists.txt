add_library(GREATEST_LIB INTERFACE "")

target_include_directories(GREATEST_LIB INTERFACE greatest)

add_library(NUKLEAR_LIB INTERFACE "")

set_target_properties(NUKLEAR_LIB PROPERTIES LINKER_LANGUAGE C)

target_include_directories(NUKLEAR_LIB INTERFACE nuklear/src)

if (!WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor wayland-egl xkbcommon)
	find_package(X11 REQUIRED)
	find_package(Doxygen)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
