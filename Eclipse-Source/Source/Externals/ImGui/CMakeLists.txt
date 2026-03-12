cmake_minimum_required(VERSION 3.19.2)
include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

set(IMGUI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE IMGUI_SOURCES
    "${IMGUI_ROOT}/*.cpp"
    "${IMGUI_ROOT}/*.h"
    "${IMGUI_ROOT}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
)



add_library(ImGui STATIC ${IMGUI_SOURCES})

if(${ECLIPSED_EDITOR})
    set_target_properties(ImGui PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs"
    )
else()
    set_target_properties(ImGui PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary"
    )
endif()

#target_precompile_headers(ImGui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(ImGui PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)

set_target_properties(ImGui PROPERTIES UNITY_BUILD OFF)
 
set_target_properties(ImGui PROPERTIES FOLDER "Externals")