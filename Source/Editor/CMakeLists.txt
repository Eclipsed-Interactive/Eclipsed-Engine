include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Editor)

GenerateExecutable(WIN32)

target_link_libraries(Editor PRIVATE 
    EclipsedEngine
    ImGui
    Reflection
)