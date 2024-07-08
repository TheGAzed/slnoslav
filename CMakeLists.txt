cmake_minimum_required(VERSION 3.22.1)

project(SLNOSLAV 
    VERSION 0 
    DESCRIPTION "A C PROJECT FOR SOLVING KAKURO PUZZLES" 
    LANGUAGES C
)

if (WIN32)
    add_compile_options(/W4)
else()
    set(CMAKE_CXX_FLAGS "-std=c23 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
    set(CMAKE_CXX_FLAGS_RELEASE "-03")
endif()

add_subdirectory(external)
add_subdirectory(program)

include(CTest)
enable_testing()
add_subdirectory(test)
