function(winDeployQt target)
    if (NOT WIN32)
        return()
    endif()

    if (NOT TARGET ${target})
        message(FATAL_ERROR "winDeployQt() was called with a non-target: ${target}")
    endif()

    get_target_property(type ${target} TYPE)
    if (NOT ${type} STREQUAL "EXECUTABLE")
        return()
    endif()

    if (NOT DEFINED Qt5_DIR)
        message(FATAL_ERROR "not defined Qt5_DIR")
    endif()

    message(STATUS "[INFO] windeployqt for target -> ${target}")

    find_program(windeployqt NAMES "windeployqt.exe" PATHS "${Qt5_DIR}/../../../bin/" REQUIRED)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND
        ${windeployqt} $<TARGET_FILE:${target}> --no-quick-import --no-translations
            --no-virtualkeyboard --no-opengl-sw --no-system-d3d-compiler --no-angle --no-webkit2
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>
        COMMAND_EXPAND_LISTS VERBATIM
    )
endfunction(winDeployQt)
