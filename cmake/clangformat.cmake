function(useClangFormat target directory)
    message(STATUS "[INFO] clang-format -> ${directory}")

    get_target_property(TYPE ${target} TYPE)
    if (${TYPE} STREQUAL "OBJECT_LIBRARY")
        message(WARNING "OBJECT_LIBRARY does not support clang-format")
        return()
    endif()

    find_program(CLANG-FORMAT_PATH clang-format REQUIRED)
    set(EXPRESSION h hpp c cc cpp cxx)
    list(TRANSFORM EXPRESSION PREPEND "${directory}/*.")
    file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS
        LIST_DIRECTORIES false ${EXPRESSION}
    )
    add_custom_command(TARGET ${target} POST_BUILD COMMAND
        ${CLANG-FORMAT_PATH} --style=file -i ${SOURCE_FILES}
        COMMAND_EXPAND_LISTS VERBATIM
    )
endfunction()
