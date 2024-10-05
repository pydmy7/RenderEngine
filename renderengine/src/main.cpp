#include "config.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>

#include "renderengine/renderengine.hpp"

auto initLog = []() -> int {
    const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        (BINARY_DIR / (std::string(PROJECT_NAME) + ".log")).string(), true);
    const auto console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

    auto logger =
        std::make_shared<spdlog::logger>("master", sinks.begin(), sinks.end());
    spdlog::set_default_logger(logger);

    return 0;
}();

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    RenderEngine::RenderEngine renderEngine;
    renderEngine.show();

    return app.exec();
}
