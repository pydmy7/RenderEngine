#include "scl/flags.hpp"
#include "scl/singleton.hpp"
#include "scl/timer.hpp"

// #include "scl/log.hpp"

// #include <spdlog/sinks/basic_file_sink.h>
// #include <spdlog/sinks/stdout_color_sinks.h>
// #include <spdlog/spdlog.h>

// void enableLoggingForCurrentUnit() {
//     const auto globalfilesink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
//         (PROJECT_BINARY_DIR / fmt::format("{}.log", PROJECT_NAME)).string(), true);
//     const auto localfilesink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
//         (CMAKE_CURRENT_BINARY_DIR / fmt::format("{}.log", CMAKE_NAME)).string(), true);
//     const auto consolesink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//     std::vector<spdlog::sink_ptr> sinks{consolesink, globalfilesink, localfilesink};

//     auto logger = std::make_shared<spdlog::logger>(
//         std::string{CMAKE_NAME}, sinks.begin(), sinks.end());
//     spdlog::set_default_logger(logger);
// }
