function(deploy target)
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
        file(GLOB_RECURSE ui "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
        set_target_properties(${target} PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

        file(GLOB_RECURSE qrc "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc")

        set_target_properties(${target} PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

        # winDeployQt(${target})
    endif()

    # Sources
    aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" sources)

    # Headers
    file(GLOB_RECURSE headers
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
    target_compile_definitions(${target} PUBLIC $<IF:$<CONFIG:Debug>,CONFIG_DEBUG,CONFIG_RELEASE>)

    if (type STREQUAL "EXECUTABLE")
        set_target_properties(${target} PROPERTIES FOLDER "Executables")
        target_include_directories(${target} PRIVATE include)
        copyDll(${target})
    else()
        set_target_properties(${target} PROPERTIES FOLDER "Libraries")
        target_include_directories(${target} PUBLIC include)
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
