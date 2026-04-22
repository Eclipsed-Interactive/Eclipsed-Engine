include("${CMAKE_SOURCE_DIR}/Tools/CMake/common.cmake")


if(INCLUDE_GAME)
    CreateProject(STATIC)
else()
    CreateProject(SHARED)
endif()

set_target_properties(EclipsedEngine PROPERTIES
    FOLDER "Engine"
)

# target_include_directories(EclipsedEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/Externals/asio")

target_link_libraries(EclipsedEngine PUBLIC 
    AssetEngine
    AudioEngine
    CoreEngine
    EntityEngine
    GraphicsEngine
    NetworkEngine
    PhysicsEngine

    ImGui
)

target_link_libraries(EclipsedEngine PRIVATE "${CMAKE_SOURCE_DIR}/Source/Externals/steamsdk/lib/win64/steam_api64.lib")


if(INCLUDE_GAME)
    target_link_libraries(EclipsedEngine PUBLIC Game)
endif()