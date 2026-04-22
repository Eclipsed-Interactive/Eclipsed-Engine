find_package(OpenGL REQUIRED)
cmake_minimum_required(VERSION 3.19.2)

set(GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
set(CMAKE_UNITY_BUILD OFF)

project(Eclipsed-Game LANGUAGES CXX)

file(GLOB_RECURSE GAME_SOURCES
    "${GAME_ROOT}/*.cpp"
    "${GAME_ROOT}/*.h"
    "${GAME_ROOT}/*.c"
    "${GAME_ROOT}/*.hpp"
    "${GAME_ROOT}/*.ipp"
    "${GAME_ROOT}/*.inl"
)

add_library(Game SHARED ${GAME_SOURCES})

target_include_directories(Game PUBLIC ${GAME_ROOT})

set(CONFIG_FILE "$ENV{APPDATA}/EclipsedEngine/EnginePath.txt")

target_link_options(Game PRIVATE /OPT:NOREF /OPT:NOICF)

if (EXISTS "${CONFIG_FILE}")
    file(READ "${CONFIG_FILE}" ENGINE_PATH)
    string(STRIP "${ENGINE_PATH}" ENGINE_PATH)

add_compile_definitions(_ECLIPSED_GAME)

set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY  "${GAME_ROOT}/Cache"
)

    target_link_libraries(Game PRIVATE 
        EclipsedEngine
        EntityEngine
        CoreEngine
        AssetEngine
        GraphicsEngine
        PhysicsEngine
        NetworkEngine

        DiscordSDK
        ImGui
        Opengl
        discord_game_sdk
        fmod_vc
        fmodL_vc
        fmodstudioL_vc
        fsbank_vc

        ${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib
        ${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/glfw3.lib
        ${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/glfw3_mt.lib

        debug ${CMAKE_SOURCE_DIR}/Tools/Libs/FreeType/freetyped.lib
        optimized ${CMAKE_SOURCE_DIR}/Tools/Libs/FreeType/freetype.lib

        debug ${CMAKE_SOURCE_DIR}/Tools/Libs/box2d/box2d-d.lib
        optimized ${CMAKE_SOURCE_DIR}/Tools/Libs/box2d/box2d.lib
        )

    target_include_directories(Game PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals)


    # target_link_directories(Game INTERFACE "${ENGINE_PATH}/Tools/Libs/fmod")

    target_link_libraries(Game INTERFACE
        "fsbank_vc"

        optimized "fmod_vc"
        optimized "fmodstudio_vc"

        debug "fmodL_vc"
        debug "fmodstudioL_vc"
    )
endif()

foreach(source_file IN LISTS GAME_SOURCES)
    file(RELATIVE_PATH source_path "${GAME_ROOT}" "${source_file}")
    get_filename_component(source_group "${source_path}" PATH)
    if(NOT source_group STREQUAL "")
        source_group("${source_group}" FILES "${source_file}")
    else()
        source_group("" FILES "${source_file}")
    endif()
endforeach()