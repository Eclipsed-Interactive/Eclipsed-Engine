include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Input)
GenerateCoreApiFile(INPUT_API)

CreateSharedProject()

target_link_libraries(Input PUBLIC 
    Core
    ImGui
)

SetFolder("Modules")