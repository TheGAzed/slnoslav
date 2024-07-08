add_library(greatest INTERFACE "")
target_include_directories(greatest INTERFACE greatest)

add_library(nuklear INTERFACE "")
target_include_directories(nuklear INTERFACE nuklear nuklear/demo/glfw_opengl3)
