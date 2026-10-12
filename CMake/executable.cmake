function(GenerateExecutable)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${ROOT}/${TARGET_NAME}/*.*"
    )

    file(GLOB_RECURSE MAIN CONFIGURE_DEPENDS
        "${ROOT}/main.cpp"
    )

    add_executable(${TARGET_NAME} ${ARGN} 
        ${SOURCES} 
        ${MAIN} 
    )

    target_sources(${TARGET_NAME}
        PRIVATE
            ${SOURCES}
            ${MAIN} 
    )

    source_group(
        TREE "${ROOT}/${TARGET_NAME}"
        FILES ${SOURCES}
    )

    source_group(
        TREE "${ROOT}"
        FILES ${MAIN}
    )

    target_include_directories(${TARGET_NAME}
        PRIVATE
            "${ROOT}"
    )

    set_target_properties(${TARGET_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin"
    )

    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin"
    
        LIBRARY_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin"
    
        RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin"
    )
endfunction()