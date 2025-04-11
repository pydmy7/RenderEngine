#include "config/config.h"

#include <client/crashpad_client.h>
#include <client/crash_report_database.h>
#include <client/settings.h>
#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/strings/string_util.h>

#include <iostream>
#include <cassert>

int main() {
    base::FilePath handler(config::PROJECT_BINARY_DIR / "vcpkg_installed/x64-windows/tools/crashpad/crashpad_handler.exe");
    base::FilePath db(config::PROJECT_BINARY_DIR / "crash_database");
    base::FilePath reports_dir(config::PROJECT_BINARY_DIR / "crash_database/none");

    std::map<std::string, std::string> annotations;
    std::vector<std::string> arguments;

    crashpad::CrashpadClient client;
    bool success = client.StartHandler(
        handler,
        db,
        reports_dir,
        "", // no metrics
        annotations,
        arguments,
        true, // restartable
        true  // asynchronous start
    );

    if (!success) {
        std::cerr << "Failed to start crashpad handler\n";
        return 1;
    }

    std::cout << "App started, will crash in 3 seconds...\n";
    Sleep(3000);

    std::cout << (config::PROJECT_BINARY_DIR / "vcpkg_installed/x64-windows/tools/crashpad/crashpad_handler.exe") << std::endl;

    // 模拟崩溃
    int* p = nullptr;
    *p = 42;
    assert(false);

    return 0;
}
