param (
    [string]$preset = "preset",
    [string]$target = "yuanshen"
)

clear
echo "clear"

echo "`npreset: $preset, target: $target"

# echo "`nRemove-Item -Recurse -Force -Path .cache"
# Remove-Item -Recurse -Force -Path .cache

# echo "`nRemove-Item -Recurse -Force -Path build"
# Remove-Item -Recurse -Force -Path build

echo "`ncmake -E make_directory ./build/.cmake/api/v1/query/client-vscode"
cmake -E make_directory ./build/.cmake/api/v1/query/client-vscode

echo "`ncmake -E capabilities | jq '.fileApi' > ./build/.cmake/api/v1/query/client-vscode/query.json"
cmake -E capabilities | jq '.fileApi' > ./build/.cmake/api/v1/query/client-vscode/query.json

echo "`nsource: cmake --preset $preset"
cmake --preset $preset

echo "`nbuild: cmake --build -j --preset $preset --target $target"
cmake --build -j --preset $preset --target $target

if ($target -eq "all") {
    exit
}

if ($?) {
    echo "`nrun: cmake -E time pwsh -Command ./build/src/$target/$target.exe"
    Invoke-Expression "cmake -E time pwsh -Command ./build/src/$target/$target.exe"
} else {
    echo "`nbuild failed!!!"
}
