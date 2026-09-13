function(SetProjectKey KEY)
    set(PROJECT_KEY ${KEY} PARENT_SCOPE)
endfunction()


function(GenerateCoreApiFile MACRO_NAME)
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_KEY}.Core.hpp"
        "#pragma once\n"
        "\n"
        "#ifdef ${PROJECT_KEY}_EXPORTS\n"
        "#define ${MACRO_NAME} __declspec(dllexport)\n"
        "#elif ${PROJECT_KEY}_IMPORTS\n"
        "#define ${MACRO_NAME} __declspec(dllimport)\n"
        "#else\n"
        "#define  ${MACRO_NAME}\n"
        "#endif"
    )
endfunction()


function(SetFolder FOLDER_STRUC)
    set_target_properties(${TARGET_NAME} PROPERTIES
        FOLDER ${FOLDER_STRUC}
    )
endfunction()


function(CreateProject TYPE)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    set(TARGET_NAME ${TARGET_NAME} PARENT_SCOPE)

    set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

    file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
        "${ROOT}/${TARGET_NAME}.Core.hpp"
        "${ROOT}/${TARGET_NAME}.pch.h"
    )

    file(GLOB_RECURSE _SOURCES CONFIGURE_DEPENDS
        "${ROOT}/${TARGET_NAME}/*.h"
        "${ROOT}/${TARGET_NAME}/*.hpp"
        "${ROOT}/${TARGET_NAME}/*.inl"
        "${ROOT}/${TARGET_NAME}/*.cpp"
        "${ROOT}/${TARGET_NAME}/*.c"
    )

    add_library(${TARGET_NAME} ${TYPE})

    target_sources(${TARGET_NAME}
        PRIVATE
            ${_SOURCES}
            ${CORE_SOURCES}
    )

    source_group(
        TREE "${ROOT}"
        FILES ${CORE_SOURCES}     
    )

    source_group(
        TREE "${ROOT}/${TARGET_NAME}"
        FILES ${_SOURCES}     
    )

    target_include_directories(${TARGET_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${ROOT}/${TARGET_NAME}>

    PUBLIC
        $<BUILD_INTERFACE:${ROOT}>
        $<INSTALL_INTERFACE:include>
)

    set_target_properties(${TARGET_NAME} PROPERTIES
        VS_INTELLECTUAL_PROPERTY "..."
    )
    

    # target_include_directories(${TARGET_NAME}
    # PUBLIC
    #     $<BUILD_INTERFACE:${ROOT}>
    #     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    # PRIVATE
    #     ${SOURCE_ROOT}
    #     ${CMAKE_SOURCE_DIR}/Source/Externals
    # )

    # if(TARGET CoreEngine AND NOT TARGET_NAME STREQUAL "CoreEngine")
    #     target_link_libraries(${TARGET_NAME}
    #         PRIVATE
    #             CoreEngine
    #     )
    # endif()


    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

    if(TYPE STREQUAL "SHARED")
        target_compile_definitions(${TARGET_NAME}
            PRIVATE
                ${TARGET_NAME}_EXPORTS

            INTERFACE
                ${TARGET_NAME}_IMPORTS
        )
    endif()

    # target_precompile_headers(${TARGET_NAME}
    #     PRIVATE
    #         ${ROOT}/pch.h
    # )

    set_target_properties(${TARGET_NAME}
        PROPERTIES
            OUTPUT_NAME "Eclipsed.${PROJECT_KEY}"
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 50
    )

    if(BUILD_EDITOR)
        set(LIB_OUTPUT
            "${CMAKE_SOURCE_DIR}/Bin/Libs/"
        )

        set(BIN_OUTPUT
            "${CMAKE_SOURCE_DIR}/Bin"
        )
    else()
        set(LIB_OUTPUT
            "${EXE_OUTPUT_DIR}/GameBuildBinary"
        )

        set(BIN_OUTPUT
            "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
        )
    endif()
    
    SetModuleOutputPaths()

endfunction()



function(SetModuleOutputPaths)
# if(BUILD_EDITOR)
    set_target_properties(${TARGET_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"

            LIBRARY_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"
            LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Libs/$<CONFIG>"

            RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin"
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin"
        )
    # else()
    #     set_target_properties(${CURRENT_DIR_NAME} PROPERTIES
    #         ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"

    #         LIBRARY_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"
    #         LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary/$<CONFIG>"

    #         RUNTIME_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
    #         RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
    #         RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
    #         RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
    #         RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/GameBuildBinary/Bin"
    #     )
    # endif()
endfunction()




function(CreateSharedProject)
    CreateProject(SHARED)
    
    set(TARGET_NAME ${TARGET_NAME} PARENT_SCOPE)
    set(ROOT ${ROOT} PARENT_SCOPE)
    set(SOURCE_ROOT ${SOURCE_ROOT} PARENT_SCOPE)
endfunction()

function(CreateStaticProject)
    CreateProject(STATIC)

    set(TARGET_NAME ${TARGET_NAME} PARENT_SCOPE)
    set(ROOT ${ROOT} PARENT_SCOPE)
    set(SOURCE_ROOT ${SOURCE_ROOT} PARENT_SCOPE)
endfunction()