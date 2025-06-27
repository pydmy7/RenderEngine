$vswhere = "vswhere.exe"

$instance = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property instanceId
if (-not $instance) {
    Write-Error "No valid Visual Studio installation instance found."
    exit 1
}

$installPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
if (-not $installPath) {
    Write-Error "Visual Studio installation path not found."
    exit 1
}

$devShellModule = Join-Path $installPath 'Common7\Tools\Microsoft.VisualStudio.DevShell.dll'
if (!(Test-Path $devShellModule)) {
    Write-Error "DevShell module not found at path: $devShellModule"
    exit 1
}

Import-Module $devShellModule

Enter-VsDevShell -InstanceId $instance -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64'

Write-Host "Successfully entered Visual Studio Developer Shell. Instance ID: $instance" -ForegroundColor Green
