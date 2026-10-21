add_library(asio INTERFACE)

target_include_directories(asio INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(asio PROPERTIES FOLDER "Externals")

    set_target_properties(asio PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_SOURCE_DIR}/Bin/Externals/Libs/$<CONFIG>"
    )
