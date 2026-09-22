cmake_minimum_required(VERSION "4.0.0")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
set(CMAKE_UNITY_BUILD ON)

option(BUILD_EDITOR "If enabled, then builds the editor." ON)

add_compile_definitions(
    WIN32_LEAN_AND_MEAN
)

if(BUILD_EDITOR)
    include(CMake/generate/editor.cmake)
else()
    include(CMake/generate/game.cmake)
endif()

project(${PROJ_NAME} LANGUAGES C CXX CSharp)

option(ENABLE_NETWORKING "Includes the network engine in game build." ON)
if(${ENABLE_NETWORKING})
    add_compile_definitions(ECLIPSED_NETWORKING)
endif()

option(ENABLE_STEAM_SDK "Enables the steam sdk." ON)
if(${ENABLE_STEAM_SDK})
    add_compile_definitions(ENABLE_STEAM_SDK)
endif()

option(ENABLE_DISCORD_SDK "Enables the discord sdk." ON)
if(${ENABLE_DISCORD_SDK})
    add_compile_definitions(ENABLE_DISCORD_SDK)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/cereal)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/FreeType)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/stb_image)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/rapidjson)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/box2d)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/asio)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/FontAwesome)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/ImGui)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Source/Externals/steamsdk/CMakeLists.txt")
    add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Externals/steamsdk)
    add_compile_definitions(STEAMSDK_PRESENT)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Core)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Assets)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Audio)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Input)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/ECS)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Renderer)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Physics)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Reflection)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Modules/Networking)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/EclipsedEngine)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Launcher)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Editor)

file(GLOB SUBDIRS LIST_DIRECTORIES true
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Plugins/*"
)

foreach(DIR ${SUBDIRS})
    if(IS_DIRECTORY "${DIR}")
        get_filename_component(DIR_NAME "${DIR}" NAME)
        string(REPLACE "-" "_" DIR_NAME "${DIR_NAME}")

        add_subdirectory("${DIR}")
  
        add_dependencies(Launcher ${DIR_NAME})
        add_dependencies(Editor ${DIR_NAME})
    endif()
endforeach()


set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT Editor
)

foreach(_tgt ALL_BUILD ZERO_CHECK)
    if(TARGET ${_tgt})
        set_target_properties(${_tgt} PROPERTIES
            VS_INTERMEDIATE_DIRECTORY "${CMAKE_BINARY_DIR}/Intermediate/${_tgt}"
        )
    endif()
endforeach()