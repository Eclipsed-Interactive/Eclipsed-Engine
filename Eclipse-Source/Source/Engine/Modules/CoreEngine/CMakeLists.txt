find_package(OpenGL REQUIRED)

include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

target_include_directories(CoreEngine PUBLIC ${DiscordSDK_SOURCES})
target_link_libraries(CoreEngine PUBLIC DiscordSDK)

target_include_directories(CoreEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(CoreEngine PRIVATE OpenGL::GL)
target_link_libraries(CoreEngine PRIVATE Opengl)
target_link_libraries(CoreEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")