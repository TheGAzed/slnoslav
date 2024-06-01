add_library(greatest "")
add_library(ubench "")

set_target_properties(greatest ubench PROPERTIES LINKER_LANGUAGE C)

target_sources(greatest
    PUBLIC
        greatest/greatest.h
)

target_sources(ubench
    PUBLIC
        ubench/ubench.h
)

target_include_directories(greatest PUBLIC greatest)
target_include_directories(ubench PUBLIC ubench)
