include_guard(GLOBAL)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

function(CollectPackageTargets directory result)
    get_property(targets DIRECTORY "${directory}" PROPERTY BUILDSYSTEM_TARGETS)
    get_property(children DIRECTORY "${directory}" PROPERTY SUBDIRECTORIES)
    foreach(child IN LISTS children)
        CollectPackageTargets("${child}" child_targets)
        list(APPEND targets ${child_targets})
    endforeach()
    set(${result} "${targets}" PARENT_SCOPE)
endfunction()

function(InstallCMakePackage)
    CollectPackageTargets("${CMAKE_SOURCE_DIR}" targets)
    foreach(target IN LISTS targets)
        get_target_property(type "${target}" TYPE)
        if(NOT type MATCHES "^(STATIC|SHARED|MODULE|INTERFACE)_LIBRARY$")
            continue()
        endif()
        # Mirror source include directories in the installed SDK.
        get_target_property(includes "${target}" INTERFACE_INCLUDE_DIRECTORIES)
        set(package_includes)
        if(includes)
            foreach(path IN LISTS includes)
                if(path MATCHES "^\\$<BUILD_INTERFACE:(.*)>$")
                    set(source_path "${CMAKE_MATCH_1}")
                elseif(IS_ABSOLUTE "${path}")
                    set(source_path "${path}")
                elseif(path MATCHES "^\\$<INSTALL_INTERFACE:")
                    continue()
                else()
                    list(APPEND package_includes "${path}")
                    continue()
                endif()
                file(RELATIVE_PATH relative "${CMAKE_SOURCE_DIR}" "${source_path}")
                if(relative MATCHES "^\\.\\.")
                    list(APPEND package_includes "${path}")
                else()
                    list(APPEND package_includes
                        "$<BUILD_INTERFACE:${source_path}>"
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Eclipsed/${relative}>")
                endif()
            endforeach()
            set_property(TARGET "${target}" PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${package_includes}")
        endif()
        # Preserve vendor import-library paths relative to the SDK prefix.
        get_target_property(links "${target}" INTERFACE_LINK_LIBRARIES)
        if(links)
            string(REPLACE "${CMAKE_SOURCE_DIR}/" "$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR}/Eclipsed/" installed_links "${links}")
            set_property(TARGET "${target}" PROPERTY INTERFACE_LINK_LIBRARIES
                "$<BUILD_INTERFACE:${links}>;$<INSTALL_INTERFACE:${installed_links}>")
        endif()
        install(TARGETS "${target}" EXPORT EclipsedTargets
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Eclipsed")
    endforeach()
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Source/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Eclipsed/Source"
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Source/"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/Eclipsed/Source"
        FILES_MATCHING PATTERN "*.lib")
    install(EXPORT EclipsedTargets FILE EclipsedTargets.cmake
        NAMESPACE Eclipsed:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Eclipsed")
    configure_package_config_file("${CMAKE_SOURCE_DIR}/CMake/Config.cmake.in"
        "${CMAKE_BINARY_DIR}/EclipsedConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Eclipsed")
    install(FILES "${CMAKE_BINARY_DIR}/EclipsedConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Eclipsed")
endfunction()
