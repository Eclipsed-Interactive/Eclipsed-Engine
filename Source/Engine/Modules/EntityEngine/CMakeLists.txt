include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

find_package(OpenGL REQUIRED)

target_include_directories(EntityEngine PRIVATE ${CoreEngine_Include})
target_link_libraries(EntityEngine PRIVATE CoreEngine)