include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

find_package(OpenGL REQUIRED)

target_include_directories(NetworkEngine PRIVATE ${CoreEngine_Include})

target_link_libraries(NetworkEngine PRIVATE CoreEngine)
target_link_libraries(NetworkEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/Externals/steamsdk/lib/win64/steam_api64.lib")

target_include_directories(NetworkEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/Engine/EclipsedEngine")

target_include_directories(NetworkEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/Externals/asio")
