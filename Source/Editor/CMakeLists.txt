include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Editor)

GenerateExecutable()

target_link_libraries(Editor PRIVATE 
    EclipsedEngine
    ImGui
    Reflection
)