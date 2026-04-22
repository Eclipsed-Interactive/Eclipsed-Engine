cmake_minimum_required(VERSION 3.19.2)

file(GLOB_RECURSE STB_IMAGE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
 
set_target_properties(stb_image PROPERTIES FOLDER "Externals")