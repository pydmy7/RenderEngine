function(copyDll target)
    if (NOT WIN32)
        return()
    endif()

    if (NOT TARGET ${target})
        message(FATAL_ERROR "copyDll() was called with a non-target: ${target}")
    endif()

    get_target_property(TYPE ${target} TYPE)
    if (NOT ${TYPE} STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "copyDll() was called on a non-executable target: ${target}")
    endif()

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying DLLs to $<TARGET_FILE_DIR:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${target}> $<TARGET_RUNTIME_DLLS:${target}>
        COMMAND_EXPAND_LISTS VERBATIM
    )
endfunction(copyDll)


function(copyFileOrDir target fileOrDir)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "Target '${target}' does not exist.")
    endif()

    if (NOT fileOrDir OR NOT EXISTS "${fileOrDir}")
        message(FATAL_ERROR "The 'fileOrDir' parameter is empty or not exist.")
    endif()

    if (IS_DIRECTORY "${fileOrDir}")
        get_filename_component(nameOnly "${fileOrDir}" NAME)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${fileOrDir} to $<TARGET_FILE_DIR:${target}>/${nameOnly}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/${nameOnly}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${fileOrDir}" "$<TARGET_FILE_DIR:${target}>/${nameOnly}"
            COMMAND_EXPAND_LISTS VERBATIM
        )
    else()
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${fileOrDir} to $<TARGET_FILE_DIR:${target}>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${fileOrDir}" "$<TARGET_FILE_DIR:${target}>"
            COMMAND_EXPAND_LISTS VERBATIM
        )
    endif()
endfunction()
