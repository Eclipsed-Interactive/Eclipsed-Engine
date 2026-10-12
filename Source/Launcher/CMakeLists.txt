include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Launcher)

GenerateExecutable(WIN32)

target_link_libraries(Launcher PRIVATE 
    EclipsedEngine
)