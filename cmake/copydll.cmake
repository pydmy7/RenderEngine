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
