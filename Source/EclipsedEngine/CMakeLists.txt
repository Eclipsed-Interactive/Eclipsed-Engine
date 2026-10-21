include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(EclipsedEngine)
GenerateCoreApiFile(ECL_API)

CreateSharedProject()

set(EXTRA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Source/Generated")

file(GLOB_RECURSE EXTRA_SOURCES CONFIGURE_DEPENDS
    "${EXTRA_SOURCE_DIR}/*.cpp"
    "${EXTRA_SOURCE_DIR}/*.h"
    "${EXTRA_SOURCE_DIR}/*.hpp"
)

target_sources(EclipsedEngine PRIVATE ${EXTRA_SOURCES})

target_include_directories(EclipsedEngine PRIVATE
    "${EXTRA_SOURCE_DIR}"
)

target_link_libraries(EclipsedEngine
    PUBLIC
        Core
        Input
        Audio
        Assets
        ECS
        Renderer
        Physics
        Networking
        Reflection

    PRIVATE
        ImGui
)