cmake_minimum_required(VERSION 3.19.2)

add_library(FMOD INTERFACE)

target_include_directories(FMOD INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(FMOD INTERFACE "${CMAKE_SOURCE_DIR}/Tools/Libs/fmod")

target_link_libraries(FMOD INTERFACE
    "fsbank_vc"

    optimized "fmod_vc"
    optimized "fmodstudio_vc"

    debug "fmodL_vc"
    debug "fmodstudioL_vc"
)

set_target_properties(FMOD PROPERTIES FOLDER "Externals")

set_target_properties(FMOD PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs"
)