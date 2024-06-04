add_library(greatest "")

set_target_properties(greatest PROPERTIES LINKER_LANGUAGE C)

target_sources(greatest
    PUBLIC
        greatest/greatest.h
)

target_include_directories(greatest PUBLIC greatest)
