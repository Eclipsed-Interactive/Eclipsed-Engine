include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Renderer)
GenerateCoreApiFile(RENDERER_API)

CreateSharedProject()

target_link_libraries(Renderer PUBLIC 
    Core
    stb_image
    Input
)

SetFolder("Modules")