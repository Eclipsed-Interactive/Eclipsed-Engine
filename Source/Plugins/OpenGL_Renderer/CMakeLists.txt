include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(OpenGL_Renderer)

CreateSharedProject()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/OpenGL")

target_link_libraries(OpenGL_Renderer 
    PUBLIC 
        EclipsedEngine

    PRIVATE
        OpenGL
        ImGui
)

SetFolder("Plugins")


get_target_property(TYPE OpenGL_Renderer TYPE)
get_target_property(LINKS OpenGL_Renderer LINK_LIBRARIES)

set_target_properties(OpenGL_Renderer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Plugins"

    LIBRARY_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Plugins"

    RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Plugins"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Plugins"
)