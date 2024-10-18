set(target renderengine)

addExecutable(${target})
target_link_libraries(${target} PRIVATE
    toml++
    spdlog::spdlog
    nlohmann::json

    Qt5::All
    hps::hps
)
copyDll(${target})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
