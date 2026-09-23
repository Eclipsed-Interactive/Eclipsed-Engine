add_library(fmod INTERFACE)

target_include_directories(fmod INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(fmod INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/Libs/fsbank_vc.lib"

    optimized "${CMAKE_CURRENT_SOURCE_DIR}/Libs/fmod_vc.lib"
    optimized "${CMAKE_CURRENT_SOURCE_DIR}/Libs/fmodstudio_vc.lib"

    debug "${CMAKE_CURRENT_SOURCE_DIR}/Libs/fmodL_vc.lib"
    debug "${CMAKE_CURRENT_SOURCE_DIR}/Libs/fmodstudioL_vc.lib"
)

set_target_properties(fmod PROPERTIES FOLDER "Externals")

    set_target_properties(fmod PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
    )