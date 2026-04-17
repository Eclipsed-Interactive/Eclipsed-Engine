cmake_minimum_required(VERSION 3.19.2)

file(GLOB_RECURSE FA
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(Font-Awesome INTERFACE)
target_include_directories(Font-Awesome INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
 
set_target_properties(Font-Awesome PROPERTIES FOLDER "Externals")