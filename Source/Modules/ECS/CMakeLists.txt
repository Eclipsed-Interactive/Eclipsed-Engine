include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(ECS)
GenerateCoreApiFile(ECS_API)

CreateSharedProject()

target_link_libraries(ECS PUBLIC 
    Core
)

SetFolder("Modules")