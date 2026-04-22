include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

find_package(OpenGL REQUIRED)

target_include_directories(GraphicsEngine PRIVATE ${CoreEngine_Include})
target_link_libraries(GraphicsEngine PRIVATE CoreEngine)

target_include_directories(GraphicsEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/FreeType)
target_link_libraries(GraphicsEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/FreeType/freetyped.lib")

target_include_directories(GraphicsEngine PUBLIC ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(GraphicsEngine PUBLIC OpenGL::GL)
target_link_libraries(GraphicsEngine PUBLIC Opengl)
target_link_libraries(GraphicsEngine PUBLIC "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")