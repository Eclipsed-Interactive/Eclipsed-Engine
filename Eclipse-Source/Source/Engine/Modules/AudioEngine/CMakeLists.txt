include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")

CreateProject(STATIC)

target_include_directories(AudioEngine PRIVATE ${CoreEngine_Include})
target_link_libraries(AudioEngine PRIVATE CoreEngine)

find_package(OpenGL REQUIRED)

target_include_directories(AudioEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL)
target_link_libraries(AudioEngine PRIVATE OpenGL::GL)
target_link_libraries(AudioEngine PRIVATE Opengl)
target_link_libraries(AudioEngine PRIVATE "${CMAKE_SOURCE_DIR}/Tools/Libs/GLFW/nvml.lib")

target_include_directories(AudioEngine PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals/fmod)  # REMOVE WHEN AUDIO ENGINE IS ADDED
target_link_libraries(AudioEngine PRIVATE FMOD)