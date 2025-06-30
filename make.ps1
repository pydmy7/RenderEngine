param (
    [string]$preset = "ninja-clang",
    [string]$config = "Debug",
    [string]$target = "yuanshen"
)

$ErrorActionPreference = "Stop"

clear
echo "clear"

echo "`npreset: $preset, config: $config, target: $target"

# echo "`nRemove-Item -Recurse -Force -Path .cache"
# Remove-Item -Recurse -Force -Path .cache

# echo "`nRemove-Item -Recurse -Force -Path build"
# Remove-Item -Recurse -Force -Path build

echo "`ncmake -E make_directory ./build/.cmake/api/v1/query/client-vscode"
cmake -E make_directory ./build/.cmake/api/v1/query/client-vscode

echo "`ncmake -E capabilities | jq '.fileApi' > ./build/.cmake/api/v1/query/client-vscode/query.json"
cmake -E capabilities | jq '.fileApi' > ./build/.cmake/api/v1/query/client-vscode/query.json

echo "`nsource: cmake --preset $preset || exit 1"
cmake --preset $preset || exit 1

echo "`nbuild: cmake --build -j --preset $preset --config $config --target $target || exit 1"
cmake --build -j --preset $preset --config $config --target $target || exit 1

if ($target -eq "all") {
    exit
}

if ($?) {
    echo "`nrun: cmake -E time pwsh -Command ./build/src/$target/$target.exe"
    Invoke-Expression "cmake -E time pwsh -Command ./build/src/$target/$config/$target.exe"
} else {
    echo "`nbuild failed!!!"
    exit 1
}
