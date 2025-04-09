#include <windows.h>
#include <dbghelp.h>

#include <string>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>

// 获取程序所在目录（使用 std::filesystem）
std::filesystem::path get_module_directory() {
    char path[MAX_PATH] = {};
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
        return std::filesystem::current_path(); // fallback
    }
    return std::filesystem::path(path).parent_path();
}

// 获取当前时间字符串（yyyy-mm-dd_HH-MM-SS）
std::string get_current_time_string() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_s(&tm_buf, &now_time);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

// 创建 Dump 文件（使用 RAII 风格）
void CreateDumpFile(EXCEPTION_POINTERS* exceptionInfo) {
    const auto dir = get_module_directory();
    const auto filename = get_current_time_string() + ".dmp";
    const auto dump_path = dir / filename;

    HANDLE hFile = CreateFileW(dump_path.wstring().c_str(),
                                GENERIC_WRITE, 0, nullptr,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create dump file: " << dump_path << '\n';
        return;
    }

    MINIDUMP_EXCEPTION_INFORMATION mdei;
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = exceptionInfo;
    mdei.ClientPointers = FALSE;

    const BOOL success = MiniDumpWriteDump(GetCurrentProcess(),
                                           GetCurrentProcessId(),
                                           hFile,
                                           MiniDumpWithFullMemory,
                                           &mdei, nullptr, nullptr);

    CloseHandle(hFile);

    if (!success) {
        std::cerr << "MiniDumpWriteDump failed.\n";
    } else {
        std::cout << "Dump file written to: " << dump_path << '\n';
    }
}

// 崩溃处理函数（SEH）
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    CreateDumpFile(exceptionInfo);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main() {
    SetUnhandledExceptionFilter(ExceptionHandler);

    // 故意崩溃
    int* p = nullptr;
    *p = *p;

    return 0;
}
