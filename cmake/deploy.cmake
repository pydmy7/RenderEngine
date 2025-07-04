function(deploy target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "deploy() was called with a non-target: ${target}")
    endif()

    # target type
    get_target_property(type ${target} TYPE)
    if (NOT (type STREQUAL "EXECUTABLE"
        OR type STREQUAL "STATIC_LIBRARY"
        OR type STREQUAL "SHARED_LIBRARY"
        OR type STREQUAL "OBJECT_LIBRARY"
    ))
        message(FATAL_ERROR "Invalid target type: ${type}")
    endif()
    message("\nTarget -> [${target}, ${type}] deploy start")

    # Qt
    if (ARGV1 STREQUAL "Qt")
        file(GLOB_RECURSE ui
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
        set_target_properties(${target} PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

        file(GLOB_RECURSE qrc
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc")

        set_target_properties(${target} PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

        # winDeployQt(${target})
    endif()

    # Sources
    aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" sources)

    # Headers
    file(GLOB_RECURSE headers
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx")

    printBuildInfo("${sources}" "${headers}" "${ui}" "${qrc}")
    target_sources(${target} PRIVATE ${sources} ${headers} ${ui} ${qrc})

    # config
    configure_file(
        "${PROJECT_SOURCE_DIR}/config/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config/config.h"
    )
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

    # compile config
    target_compile_features(${target} PUBLIC cxx_std_20)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/permissive;/Zc:preprocessor;/utf-8;/W4;>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Wshadow;>
    )

    if (type STREQUAL "EXECUTABLE")
        set_target_properties(${target} PROPERTIES FOLDER "Executables")
        target_include_directories(${target} PRIVATE include)
        copyDll(${target})
        generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")
    else()
        set_target_properties(${target} PROPERTIES FOLDER "Libraries")
        target_include_directories(${target} PUBLIC include)

        if (type STREQUAL "SHARED_LIBRARY")
            set_target_properties(${target} PROPERTIES
                OUTPUT_NAME_DEBUG "${target}d"
            )
        elseif (type STREQUAL "STATIC_LIBRARY")
            set_target_properties(${target} PROPERTIES
                OUTPUT_NAME "${target}-static"
            )
        endif()
    endif()

    # useClangFormat(${target} ${CMAKE_CURRENT_SOURCE_DIR})
    # useClangTidy(${target})

    message("Target -> [${target}, ${type}] deploy end\n")
endfunction(deploy)


function(printBuildInfo sources headers ui qrc)
    if (sources)
        message(STATUS "Sources:")
        foreach(file IN LISTS sources)
            message(STATUS " -> ${file}")
        endforeach()
    endif()
    if (headers)
        message(STATUS "Headers:")
        foreach(file IN LISTS headers)
            message(STATUS " -> ${file}")
        endforeach()
    endif()
    if (ui)
        message(STATUS "UI Files:")
        foreach(file IN LISTS ui)
            message(STATUS " -> ${file}")
        endforeach()
    endif()
    if (qrc)
        message(STATUS "QRC Files:")
        foreach(file IN LISTS qrc)
            message(STATUS " -> ${file}")
        endforeach()
    endif()
endfunction(printBuildInfo)
