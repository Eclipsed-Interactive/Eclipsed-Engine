cmake_minimum_required(VERSION 3.19.2)

# file(READ "${CMAKE_SOURCE_DIR}/Settings/EngineSettings.json" GAME_NAME_FILE)
# string(JSON GameJSONName GET "${GAME_NAME_FILE}" GameName)
set(GAME_NAME "Eclipsed Engine")

set(LAUNCHER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE LAUNCHER_SOURCES
    "${LAUNCHER_ROOT}/main.cpp"
)

add_executable(Launcher "${LAUNCHER_ROOT}/main.cpp")

target_include_directories(Launcher PUBLIC ${CMAKE_SOURCE_DIR}/Source/Launcher/)
    target_include_directories(Launcher PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals)

target_link_libraries(Launcher PRIVATE EclipsedEngine)

if(ECLIPSED_EDITOR)
    set(OUTPUT ${EXE_OUTPUT_DIR})
    
else()
    set(OUTPUT ${BUILD_GAME_PATH})
set_target_properties(Launcher PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${BUILD_GAME_PATH}"
    )
endif()

set_target_properties(Launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT}"
    OUTPUT_NAME "${GAME_NAME}_$<CONFIG>"
)



set_target_properties(Launcher PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 50)