include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Reflection)
GenerateCoreApiFile(REFLECTION_API)

CreateSharedProject()

target_link_libraries(Reflection PUBLIC 
    Core 
)

SetFolder("Modules")