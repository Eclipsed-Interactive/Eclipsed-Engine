include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

find_package(OpenGL REQUIRED)

target_include_directories(AssetEngine PRIVATE ${CoreEngine_Include})
target_link_libraries(AssetEngine PRIVATE CoreEngine)

target_include_directories(AssetEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(AssetEngine PRIVATE OpenGL::GL)
target_link_libraries(AssetEngine PRIVATE Opengl)
target_link_libraries(AssetEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")

# ---- Freetype
target_include_directories(AssetEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/FreeType)
target_link_libraries(AssetEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/FreeType/freetyped.lib")


# ---- OpenGL and GLFW
target_include_directories(AssetEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(AssetEngine PRIVATE OpenGL::GL)
target_link_libraries(AssetEngine PRIVATE Opengl)
target_link_libraries(AssetEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")


target_include_directories(AssetEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/fmod)  # REMOVE WHEN AUDIO ENGINE IS ADDED
target_link_libraries(AssetEngine PRIVATE FMOD)