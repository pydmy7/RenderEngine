#include "config.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <QApplication>

#include "renderengine/renderengine.hpp"

auto initLog = []() -> int {
    const auto file_logger {
        spdlog::basic_logger_mt(
            "master",
            (BINARY_DIR / (std::string(PROJECT_NAME) + ".log")).string()
        )
    };
    spdlog::set_default_logger(file_logger);
    return 0;
}();

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    RenderEngine::RenderEngine renderEngine;
    renderEngine.show();

    return app.exec();
}
