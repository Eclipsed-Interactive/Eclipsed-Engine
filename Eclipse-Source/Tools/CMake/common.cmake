# function(AddUnityTargetEXE TARGET_NAME)
#     set(UNITY_BUILD_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_unity.cpp)
    
#     file(WRITE ${UNITY_BUILD_FILE} "// Unity build file generated through cmake for ${TARGET_NAME}\n\n")
#     foreach(SRC_FILE ${ARGN})
#         file(APPEND ${UNITY_BUILD_FILE} "#include \"${SRC_FILE}\"\n")
#     endforeach()
    
#     if(WIN32)
#         add_executable(${TARGET_NAME} WIN32 ${UNITY_BUILD_FILE})
#     endif(WIN32)
#     if(UNIX)
#         add_executable(${TARGET_NAME} ${UNITY_BUILD_FILE})
#     endif(UNIX)

#     target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# endfunction()

# function(AddUnityTargetLIB TARGET_NAME)
#     set(UNITY_BUILD_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_unity.cpp)
    
#     file(WRITE ${UNITY_BUILD_FILE} "// Unity build file generated through cmake for ${TARGET_NAME}\n\n")
#     foreach(SRC_FILE ${ARGN})
#         file(APPEND ${UNITY_BUILD_FILE} "#include \"${SRC_FILE}\"\n")
#     endforeach()
    
#     add_library(${TARGET_NAME} STATIC ${UNITY_BUILD_FILE})

#     target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# endfunction()


# function(AddFilesToFolders SOURCES REL_PATH)
#     foreach(source_file IN LISTS SOURCES)
#         file(RELATIVE_PATH source_path "${REL_PATH}" "${source_file}")
#         get_filename_component(source_group "${source_path}" PATH)
#         if(NOT source_group STREQUAL "")
#             source_group("${source_group}" FILES "${source_file}")
#         else()
#             source_group("" FILES "${source_file}")
#         endif()
#     endforeach()
# endfunction()


function(AddFilesToFolders SOURCES REL_PATH)
    get_filename_component(base_name "${REL_PATH}" NAME)
    foreach(source_file IN LISTS SOURCES)
        file(RELATIVE_PATH source_path "${REL_PATH}" "${source_file}")
        get_filename_component(source_group "${source_path}" PATH)
        
        # Remove duplicate top-level folder if it matches base folder
        string(REGEX REPLACE "^${base_name}[\\/]" "" source_group "${source_group}")
        
        if(NOT source_group STREQUAL "")
            source_group("${source_group}" FILES "${source_file}")
        else()
            source_group("" FILES "${source_file}")
        endif()
    endforeach()
endfunction()



set(BINARY_PATH ${CMAKE_SOURCE_DIR}/Library)


function(CreateProject TYPE)
    cmake_minimum_required(VERSION 3.19.2)

    get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${CURRENT_DIR_NAME}")
    file(GLOB_RECURSE SOURCES
        "${ROOT}/*.cpp"
        "${ROOT}/*.c"
        "${ROOT}/*.h"
        "${ROOT}/*.hpp"
        "${ROOT}/*.inl"
    )


    set(${CURRENT_DIR_NAME}_Include ${SOURCES})

    add_library(${CURRENT_DIR_NAME} ${TYPE})


    target_sources(${CURRENT_DIR_NAME} PRIVATE ${SOURCES})
    target_sources(${CURRENT_DIR_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")
        
    target_include_directories(${CURRENT_DIR_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    target_include_directories(${CURRENT_DIR_NAME} PRIVATE "${ROOT}")

    target_precompile_headers(${CURRENT_DIR_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

        set_target_properties(${CURRENT_DIR_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs"
            ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs"
            ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs"

            LIBRARY_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}/Libs"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}/Libs"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}/Libs"
            LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}/Libs"
            LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}/Libs"

        RUNTIME_OUTPUT_DIRECTORY                "${EXE_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${EXE_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${EXE_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXE_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${EXE_OUTPUT_DIR}"

        FOLDER "Engine/EngineModules"
    )

    target_include_directories(${CURRENT_DIR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source/Externals)

    source_group(TREE "${ROOT}" FILES ${SOURCES})

    set_target_properties(${CURRENT_DIR_NAME} PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 50)
endfunction()