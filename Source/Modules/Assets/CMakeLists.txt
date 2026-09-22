include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Assets)
GenerateCoreApiFile(ASSET_API)

CreateSharedProject()

target_link_libraries(Assets PUBLIC 
    Core 
    FreeType
    OpenGL
    fmod
)

SetFolder("Modules")