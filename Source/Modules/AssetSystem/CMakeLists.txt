include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(AssetSystem)
GenerateCoreApiFile(ASSETSYSTEM_API)

CreateSharedProject()

target_link_libraries(AssetSystem PUBLIC 
    Core 
    FreeType
    OpenGL
    fmod
    Audio
    Renderer
    Audio
    Serialization
)

SetFolder("Modules")