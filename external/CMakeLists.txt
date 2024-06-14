add_library(GREATEST_LIB INTERFACE "")

target_include_directories(GREATEST_LIB INTERFACE greatest)

add_library(NUKLEAR_LIB INTERFACE "")

set_target_properties(NUKLEAR_LIB PROPERTIES LINKER_LANGUAGE C)

target_include_directories(NUKLEAR_LIB INTERFACE nuklear/src)
