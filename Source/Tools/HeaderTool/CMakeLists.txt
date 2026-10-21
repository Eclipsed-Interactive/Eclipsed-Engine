include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

set(TARGET_NAME "HeaderTool")

SetProjectKey(HeaderTool)

GenerateExecutable()

target_link_libraries(HeaderTool PRIVATE 
    Core
    Reflection
)

SetFolder("Tools")