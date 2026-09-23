include("${CMAKE_SOURCE_DIR}/CMake/common.cmake")

SetProjectKey(Audio_FMOD)

CreateSharedProject()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/fmod")

target_link_libraries(Audio_FMOD 
    PUBLIC 
        EclipsedEngine

    PRIVATE
        fmod
        Assets
        Core
)

SetFolder("Plugins")


get_target_property(TYPE Audio_FMOD TYPE)
get_target_property(LINKS Audio_FMOD LINK_LIBRARIES)

set_target_properties(Audio_FMOD PROPERTIES
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