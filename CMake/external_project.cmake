function(SetProjectKey KEY)
    set(PROJECT_KEY ${KEY} PARENT_SCOPE)
endfunction()


function(CreateExternalInterfaceProject)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

    add_library(${TARGET_NAME} INTERFACE)

    target_include_directories(${TARGET_NAME}
        INTERFACE
            $<BUILD_INTERFACE:${ROOT}>
            $<INSTALL_INTERFACE:include/${TARGET_NAME}>
    )

    set_target_properties(${TARGET_NAME}
        PROPERTIES
            FOLDER "Externals"
    )
endfunction()


function(CreateExternalProject)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    set(TARGET_NAME ${TARGET_NAME})
    set(TARGET_NAME ${TARGET_NAME} PARENT_SCOPE)

    set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ROOT "${ROOT}" PARENT_SCOPE)

    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${ROOT}/*.cpp"
        "${ROOT}/*.c"
        "${ROOT}/*.h"
        "${ROOT}/*.hpp"
        "${ROOT}/*.inl"
    )

    source_group(TREE "${ROOT}" FILES ${SOURCES})

    add_library(${TARGET_NAME} STATIC)

    set(HEADER_FILES ${SOURCES})
    list(FILTER HEADER_FILES INCLUDE REGEX "\\.(h|hpp|inl)$")

    set(SOURCE_FILES ${SOURCES})
    list(FILTER SOURCE_FILES INCLUDE REGEX "\\.(cpp|c)$")

    target_sources(${TARGET_NAME}
        PRIVATE
            ${SOURCE_FILES}

        PUBLIC
            FILE_SET HEADERS
            TYPE HEADERS
            BASE_DIRS
                ${ROOT}
            FILES
                ${HEADER_FILES}
    )

target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${ROOT}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${ROOT}

    PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Externals
)

    # target_precompile_headers(${TARGET_NAME}
    #     PRIVATE
    #         ${ROOT}/${TARGET_NAME}.h
    # )

    set_target_properties(${TARGET_NAME}
        PROPERTIES
            OUTPUT_NAME "Eclipsed.External.${PROJECT_KEY}"
            FOLDER "Externals"
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 50
    )

    if(ECLIPSED_EDITOR)
        set(LIB_OUTPUT
            "${EXE_OUTPUT_DIR}/Libs/"
        )

        set(BIN_OUTPUT
            "${EXE_OUTPUT_DIR}"
        )
    else()
        set(LIB_OUTPUT
            "${EXE_OUTPUT_DIR}/GameBuildBinary"
        )

        set(BIN_OUTPUT
            "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
        )
    endif()

    SetExternalOutputPaths()
endfunction()



function(SetExternalOutputPaths)
    if(ECLIPSED_EDITOR)
        set_target_properties(${CURRENT_DIR_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs/$<CONFIG>/Externals"

            FOLDER "Externals"
        )
    else()
        set_target_properties(${CURRENT_DIR_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>/Externals"
            ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>/Externals"

            FOLDER "Externals"
        )
    endif()
endfunction()