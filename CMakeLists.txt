cmake_minimum_required(VERSION "4.0.0")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
set(CMAKE_UNITY_BUILD ON)

# Or set a common base directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/custom-build)

project(Eclipsed-Engine LANGUAGES CXX)

add_compile_definitions(LEAN_AND_MEAN)

option(ECLIPSED_EDITOR "Enables the editor" TRUE)
option(INCLUDE_GAME "Includes game" FALSE)
option(BUILD_GAME_PATH "Includes game" "${CMAKE_SOURCE_DIR}/Bin")

if(${ECLIPSED_EDITOR})
    add_compile_definitions(
        ECLIPSED_EDITOR
        _ECLIPSED_EXPORT_DLL
    )
else(    )
    add_compile_definitions(_NO_EXPORT)
endif()


set_property(GLOBAL PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin")


set(LAUNCH_ARGS "--defaultArg" CACHE STRING "Arguments to pass to launcher")

# if(NOT DEFINED PROJECT_DIR OR PROJECT_DIR STREQUAL "")
#     file(READ "${EXE_OUTPUT_DIR}/.ini" PROJECT_DIR)
#     string(REPLACE "\n" "" PROJECT_DIR "${PROJECT_DIR}")
#     string(REPLACE " " "" PROJECT_DIR "${PROJECT_DIR}")
# else()
#     set(PROJECT_DIR "${PROJECT_DIR}" CACHE PATH "Where to place executables")
# endif()




# Declare cache variable with a default value if not set
if(NOT DEFINED EXE_OUTPUT_DIR OR EXE_OUTPUT_DIR STREQUAL "")
    set(EXE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Bin" CACHE PATH "Where to place executables" FORCE)
else()
    # Make sure it's in the cache without overwriting
    set(EXE_OUTPUT_DIR "${EXE_OUTPUT_DIR}" CACHE PATH "Where to place executables")
endif()

# set(EXE_OUTPUT_DIR "${EXE_OUTPUT_DIR}"
#     CACHE PATH "Where to place executables")

# if(NOT EXE_OUTPUT_DIR)
#     set(EXE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Bin" CACHE PATH "Where to place executables")
# endif()


add_compile_definitions(BEGIN_IMPL=//)
add_compile_definitions(END_IMPL=//)


# ---- Externals
add_subdirectory(Source/Externals/DiscordSDK)
add_subdirectory(Source/Externals/fmod)
add_subdirectory(Source/Externals/rapidjson)
add_subdirectory(Source/Externals/ImGui)
add_subdirectory(Source/Externals/OpenGL)
add_subdirectory(Source/Externals/SDL3)
add_subdirectory(Source/Externals/stb_image)
add_subdirectory(Source/Externals/Font-Awesome)



add_subdirectory(Source/Engine/Modules/CoreEngine)
add_subdirectory(Source/Engine/Modules/AssetEngine)
add_subdirectory(Source/Engine/Modules/AudioEngine)
add_subdirectory(Source/Engine/Modules/EntityEngine)
add_subdirectory(Source/Engine/Modules/GraphicsEngine)
add_subdirectory(Source/Engine/Modules/NetworkEngine)
add_subdirectory(Source/Engine/Modules/PhysicsEngine)

add_subdirectory(Source/Engine/EclipsedEngine)

add_subdirectory(Source/Launcher)
if(ECLIPSED_EDITOR)
    add_subdirectory(Source/Hub)
endif()

if(INCLUDE_GAME)
    add_subdirectory(${PROJECT_DIR}  ${PROJECT_DIR}/Library/Game)
endif()

message(${CMAKE_SOURCE_DIR})

# add_custom_target(post_build
#     COMMAND python ${CMAKE_SOURCE_DIR}/Tools/Build/export-headers.py
#             --config $<CONFIG>
#             --input "${CMAKE_SOURCE_DIR}/Source/Engine"
#             --output "${CMAKE_SOURCE_DIR}/Bin/Headers"
#     COMMENT "Running post-build script"
#     DEPENDS EclipsedEngine Launcher
# )





set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Launcher)

make_directory("${CMAKE_SOURCE_DIR}/Bin")

file(GLOB DLLs "${CMAKE_SOURCE_DIR}/Assets/DLLs/*")
file(COPY ${DLLs} DESTINATION "${CMAKE_SOURCE_DIR}/Bin")

make_directory("${CMAKE_SOURCE_DIR}/Bin/Local")
file(GLOB DLLs "${CMAKE_SOURCE_DIR}/Assets/Settings/*")
file(COPY ${DLLs} DESTINATION "${CMAKE_SOURCE_DIR}/Bin/Local")

file(GLOB EngineAssets "${CMAKE_SOURCE_DIR}/Assets/EngineAssets")
file(COPY ${EngineAssets} DESTINATION "${CMAKE_SOURCE_DIR}/Bin")

