cmake_minimum_required(VERSION 3.19.2)


set(ASSET_EXPORT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE ASSET_EXPORT_SOURCES
    "${ASSET_EXPORT_ROOT}/main.cpp"
)

add_executable(EclipsedHub "${ASSET_EXPORT_ROOT}/main.cpp")

target_include_directories(EclipsedHub PUBLIC ${CMAKE_SOURCE_DIR}/Source/Tools/EclipsedHub/)
    target_include_directories(EclipsedHub PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals)

    target_link_libraries(EclipsedHub PRIVATE AssetEngine)
    target_link_libraries(EclipsedHub PRIVATE CoreEngine)
    
set_target_properties(EclipsedHub PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${EXE_OUTPUT_DIR}"
RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXE_OUTPUT_DIR}"
RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXE_OUTPUT_DIR}"
OUTPUT_NAME "${GAME_NAME}_$<CONFIG>"
VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin"
)

set_target_properties(EclipsedEngine PROPERTIES
    CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/EclipsedHub"
    CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Bin/EclipsedHub"
    CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Bin/EclipsedHub"
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/EclipsedHub"
    CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/EclipsedHub"
    FOLDER "Tools"
)




set_target_properties(EclipsedHub PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 50)