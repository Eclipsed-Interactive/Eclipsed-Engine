cmake_minimum_required(VERSION 3.19.2)

set(HUB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE HUB_SOURCES
    "${HUB_ROOT}/*.cpp"
    "${HUB_ROOT}/*.c"
    "${HUB_ROOT}/*.h"
    "${HUB_ROOT}/*.hpp"
    "${HUB_ROOT}/*.inl"
)

add_executable(Hub WIN32 "${HUB_SOURCES}")

# message(${HUB_SOURCES})

target_include_directories(Hub PRIVATE "${HUB_ROOT}")
    target_include_directories(Hub PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals)

    add_compile_definitions(ECLIPSED_HUB)
# target_link_libraries(Hub PRIVATE EclipsedEngine)

if(ECLIPSED_EDITOR)
    set(OUTPUT ${EXE_OUTPUT_DIR})
else()
    set(OUTPUT ${BUILD_GAME_PATH})
endif()

target_link_libraries(Hub PUBLIC 
    CoreEngine

    ImGui
)

add_dependencies(Hub Launcher)

target_include_directories(Hub PUBLIC ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(Hub PUBLIC Opengl)
target_link_libraries(Hub PUBLIC "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")

set_target_properties(Hub PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT}"
    OUTPUT_NAME "Eclipsed_Hub_$<CONFIG>"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin"
)

set_target_properties(Hub PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 50)