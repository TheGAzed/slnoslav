add_library(greatest "")

set_target_properties(greatest PROPERTIES LINKER_LANGUAGE C)

target_sources(greatest
    PUBLIC
        greatest/greatest.h
)

target_include_directories(greatest PUBLIC greatest)

add_library(nuklear "")

set_target_properties(nuklear PROPERTIES LINKER_LANGUAGE C)

target_sources(nuklear
    PUBLIC
        nuklear/nuklear.h
)

target_include_directories(nuklear PUBLIC nuklear/src)
