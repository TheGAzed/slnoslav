cmake_minimum_required(VERSION 3.22.1)

project(SLNOSLAV 
    VERSION 0 
    DESCRIPTION "A C PROJECT FOR SOLVING KAKURO PUZZLES" 
    LANGUAGES C
)

if(MSVC)
    set(CMAKE_C_FLAGS "-std=c17 -pg -Wall")
else()
    set(CMAKE_C_FLAGS "-std=c17 -pg -Wall -Werror")
endif()

add_subdirectory(external)
add_subdirectory(program)
add_subdirectory(test)
