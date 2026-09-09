find_package(OpenGL REQUIRED)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB_RECURSE OPENGL_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}/*.*"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.*"
)

add_library(OpenGL STATIC ${OPENGL_SOURCES})

#target_precompile_headers(OpenGL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_include_directories(OpenGL 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/ImGui"
        "${CMAKE_CURRENT_SOURCE_DIR}/OpenGL"
)

file(GLOB LIB_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Libs/*.lib"
)

target_link_libraries(OpenGL INTERFACE
    ${LIB_FILES}
    
    PRIVATE
        ImGui
        cereal
)

if(BUILD_EDITOR)
    set_target_properties(OpenGL PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Libs/Externals/$<CONFIG>"
    )
else()
    set_target_properties(OpenGL PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_CURRENT_SOURCE_DIR}/GameBuildBinary/Externals/"
    )
endif()

set_target_properties(OpenGL PROPERTIES FOLDER "Externals")

set_target_properties(OpenGL PROPERTIES UNITY_BUILD OFF UNITY_BUILD_BATCH_SIZE 50)

set_source_files_properties("${CMAKE_SOURCE_DIR}/Source/Externals/OpenGL/glad/glad.c" PROPERTIES LANGUAGE C)
set_target_properties(OpenGL PROPERTIES C_STANDARD 11)
set_target_properties(OpenGL PROPERTIES FOLDER "Externals")