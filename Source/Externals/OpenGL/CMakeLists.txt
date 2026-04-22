find_package(OpenGL REQUIRED)
include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

cmake_minimum_required(VERSION 3.19.2)

file(GLOB_RECURSE OPENGL_SOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c"
"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
)

add_library(Opengl STATIC ${OPENGL_SOURCES})

#target_precompile_headers(Opengl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_include_directories(Opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Opengl INTERFACE
    "${CMAKE_SOURCE_DIR}/Thirdparty/Libs/GLFW/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/Thirdparty/Libs/GLFW/glfw3_mt.lib"
)

if(${ECLIPSED_EDITOR})
    set_target_properties(Opengl PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs"
    )
else()
    set_target_properties(Opengl PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary"
    )
endif()

set_target_properties(Opengl PROPERTIES FOLDER "Externals")

set_target_properties(Opengl PROPERTIES UNITY_BUILD OFF UNITY_BUILD_BATCH_SIZE 50)


set_source_files_properties("${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL/glad/glad.c" PROPERTIES LANGUAGE C)
set_target_properties(Opengl PROPERTIES LINKER_LANGUAGE C)
set_target_properties(Opengl PROPERTIES C_STANDARD 11)
set_target_properties(Opengl PROPERTIES FOLDER "Externals")

add_custom_command(TARGET Opengl POST_BUILD
    COMMAND python ${CMAKE_SOURCE_DIR}/Tools/Build/post_build.py
            "${CMAKE_SOURCE_DIR}/Source/Engine"
            "${CMAKE_SOURCE_DIR}/Bin/Headers"
            "${CMAKE_SOURCE_DIR}/Assets"
    COMMENT "Running post-build script"
)