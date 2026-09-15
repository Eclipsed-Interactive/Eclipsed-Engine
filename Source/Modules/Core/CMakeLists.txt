include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Core)
GenerateCoreApiFile(CORE_API)

CreateSharedProject()

target_link_libraries(Core
    PUBLIC
        cereal
        rapidjson
        FontAwesome
        stb_image
)

SetFolder("Modules")
