function getTargetPath {
    param (
        [string]$filePath,
        [string]$config,
        [string]$target
    )

    $key = "$config-$target-Path"

    if (-Not (Test-Path $filePath)) {
        throw "Configuration file '$filePath' does not exist."
    }

    $lines = Get-Content $filePath

    foreach ($line in $lines) {
        if ($line.Trim() -eq "" -or $line.Trim().StartsWith("#")) {
            continue
        }

        if ($line -match "^\s*$key\s*=\s*""([^""]+)""\s*$") {
            return $matches[1]
        }
    }

    throw "Key '$key' not found in file."
}
