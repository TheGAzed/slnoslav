add_library(GREATEST_LIB "")

set_target_properties(GREATEST_LIB PROPERTIES LINKER_LANGUAGE C)

target_sources(GREATEST_LIB
    PUBLIC
        greatest/greatest.h
)

target_include_directories(GREATEST_LIB PUBLIC greatest)

add_library(NUKLEAR_LIB "")

set_target_properties(NUKLEAR_LIB PROPERTIES LINKER_LANGUAGE C)

target_sources(NUKLEAR_LIB
    PUBLIC
        nuklear/nuklear.h
)

target_include_directories(NUKLEAR_LIB PUBLIC nuklear/src)
