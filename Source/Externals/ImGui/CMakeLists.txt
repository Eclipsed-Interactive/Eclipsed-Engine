get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB_RECURSE IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}/*.*"
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_link_libraries(ImGui
    PRIVATE
        OpenGL
)

target_include_directories(ImGui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}
)

set_target_properties(ImGui PROPERTIES
    FOLDER "Externals"
    EXCLUDE_FROM_ALL TRUE

    ARCHIVE_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"

    VS_INTERMEDIATE_DIRECTORY
        "${CMAKE_BINARY_DIR}/Intermediate/ImGui"
)