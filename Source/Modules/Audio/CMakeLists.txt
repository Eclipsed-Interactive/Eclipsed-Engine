include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Audio)
GenerateCoreApiFile(AUDIO_API)

CreateSharedProject()

target_link_libraries(Audio
    PUBLIC
        Core
)

SetFolder("Modules")