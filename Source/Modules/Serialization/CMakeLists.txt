include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Serialization)
GenerateCoreApiFile(SERIALIZATION_API)

CreateSharedProject()

target_link_libraries(Serialization PUBLIC 
    Core 
    ImGui
    Reflection
)

SetFolder("Modules")