cmake_minimum_required(VERSION 3.19.2)

file(GLOB_RECURSE SDL3_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(SDL3 INTERFACE)
target_include_directories(SDL3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
 
target_link_libraries(SDL3 INTERFACE
    "${CMAKE_SOURCE_DIR}/Tools/Libs/sdl3/SDL3.lib"
)

set_target_properties(SDL3 PROPERTIES FOLDER "Externals")