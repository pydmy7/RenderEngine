function(generatePath target_name file_path)
    if (NOT TARGET ${target_name})
        message(FATAL_ERROR "Target '${target_name}' does not exist.")
    endif()

    get_target_property(target_type ${target_name} TYPE)
    if (NOT ${target_type} STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "Target '${target_name}' is not an executable.")
    endif()

    file(GENERATE
        OUTPUT "${CMAKE_BINARY_DIR}/PathInfo/$<CONFIG>-${target_name}.toml"
        CONTENT "$<CONFIG>-${target_name}-Path = \"$<TARGET_FILE:${target_name}>\"\n"
        TARGET ${target_name}
    )

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Merging generate paths..."
        COMMAND ${CMAKE_COMMAND}
            -DfolderPath="${CMAKE_BINARY_DIR}/PathInfo/"
            -DdestPath="${file_path}"
            -P "${PROJECT_SOURCE_DIR}/cmake/mergefiles.cmake"
    )
endfunction(generatePath)
