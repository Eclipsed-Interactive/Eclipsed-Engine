cmake_minimum_required(VERSION 3.19.2)
include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

file(GLOB_RECURSE DiscordSDK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
)

add_library(DiscordSDK STATIC ${DiscordSDK_SOURCES})

target_precompile_headers(DiscordSDK PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")


if(${ECLIPSED_EDITOR})
    set_target_properties(DiscordSDK PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs"
    )
else()
    set_target_properties(DiscordSDK PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary"
    )
endif()





target_include_directories(DiscordSDK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_directories(DiscordSDK PUBLIC "${CMAKE_SOURCE_DIR}/Tools/Libs/discord_sdk")
target_link_libraries(DiscordSDK PUBLIC "discord_game_sdk")

set_target_properties(DiscordSDK PROPERTIES FOLDER "Externals")