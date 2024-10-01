add_library(spdlog::spdlog STATIC IMPORTED GLOBAL)
set_target_properties(
    spdlog::spdlog
    PROPERTIES

    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"

    IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlogd.lib"
    IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/lib/spdlog.lib"
)
