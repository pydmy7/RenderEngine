param (
    [string]$compiler = "Clang"
)

clear
echo "clear"

echo "`nRemove-Item -Path ./build -Recurse -Force"
Remove-Item -Path ./build -Recurse -Force

if ($compiler -eq "Clang") {
    echo "`nsource: cmake -S . -B build -G Ninja"
    cmake -S . -B build -G Ninja

    echo "`nbuild: cmake --build build -j"
    cmake --build build -j

    echo "`nrun: ./build/renderengine/renderengine.exe"
    ./build/renderengine/renderengine.exe
} elseif ($compiler -eq "MSVC") {
    echo "`nsource: cmake -S . -B build"
    cmake -S . -B build

    echo "`nbuild: cmake --build build -j"
    cmake --build build -j

    echo "`nrun: ./build/renderengine/Debug/renderengine.exe"
    ./build/renderengine/Debug/renderengine.exe
} else {
    echo "Unknown compiler specified. Please use Clang or MSVC."
}
