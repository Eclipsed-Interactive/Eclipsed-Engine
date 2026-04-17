cmake_minimum_required(VERSION 3.19.2)

file(GLOB_RECURSE RAPIDJSON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
 
set_target_properties(rapidjson PROPERTIES FOLDER "Externals")