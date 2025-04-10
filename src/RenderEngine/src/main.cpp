#include <client/crashpad_client.h>
#include <client/crash_report_database.h>
#include <client/settings.h>
#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/strings/string_util.h>

#include <iostream>

int main() {
    base::FilePath handler(L"D:\\Code\\RenderEngine\\build\\vcpkg_installed\\x64-windows\\debug\\tools\\crashpad_handler.exe"); // 替换成你实际的路径
    base::FilePath db(L"D:\\Code\\RenderEngine\\build\\dmp");
    base::FilePath reports_dir(L"D:\\Code\\RenderEngine\\build\\dmp\\reports");

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

    // 模拟崩溃
    volatile int* p = nullptr;
    *p = 42;

    return 0;
}
