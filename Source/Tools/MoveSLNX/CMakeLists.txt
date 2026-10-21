include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

set(TARGET_NAME "MoveSLNX")

SetProjectKey(MoveSLNX)

GenerateExecutable()

target_link_libraries(MoveSLNX PRIVATE 
    Core
)

SetFolder("Tools")