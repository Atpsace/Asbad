# build.ps1 — portable build for the usermode overlay sources
# Resolves paths from this script's directory instead of a hardcoded user profile.

param(
    [string]$OutputName = "esp_fast.exe"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$Output = Join-Path $Root $OutputName

$CompilerCandidates = @(
    "g++",
    "C:\msys64\ucrt64\bin\g++.exe",
    "C:\msys64\mingw64\bin\g++.exe"
)
$Compiler = $null
foreach ($c in $CompilerCandidates) {
    if ($c -eq "g++") {
        $cmd = Get-Command g++ -ErrorAction SilentlyContinue
        if ($cmd) { $Compiler = $cmd.Source; break }
    } elseif (Test-Path $c) {
        $Compiler = $c
        break
    }
}

if (-not $Compiler) {
    Write-Host "[-] g++ not found. Install MSYS2 ucrt64 or add g++ to PATH." -ForegroundColor Red
    exit 1
}

$Includes = @(
    "-I`"$Root`"",
    "-I`"$Root\ImGui LianFlow`"",
    "-I`"$Root\ImGui LianFlow\backends`"",
    "-I`"$Root\ImGui LianFlow\examples\example_win32_directx11`""
)

$Sources = @(
    "`"$Root\main.cpp`"",
    "`"$Root\ImGui LianFlow\imgui.cpp`"",
    "`"$Root\ImGui LianFlow\imgui_draw.cpp`"",
    "`"$Root\ImGui LianFlow\imgui_widgets.cpp`"",
    "`"$Root\ImGui LianFlow\imgui_tables.cpp`"",
    "`"$Root\ImGui LianFlow\backends\imgui_impl_win32.cpp`"",
    "`"$Root\ImGui LianFlow\backends\imgui_impl_dx11.cpp`"",
    "`"$Root\ImGui LianFlow\examples\example_win32_directx11\custom_widgets.cpp`""
)

$Libs = "-ld3d11 -ld3dcompiler -ldwmapi -lgdi32 -luser32 -lwinmm -lole32 -ladvapi32 -lntdll -mwindows"

Write-Host "[*] Compiler: $Compiler"
Write-Host "[*] Output:   $Output"

$argLine = "-std=c++17 -O2 -msse4.1 " + ($Includes -join " ") + " " + ($Sources -join " ") + " -o `"$Output`" " + $Libs
$p = Start-Process -FilePath $Compiler -ArgumentList $argLine -NoNewWindow -Wait -PassThru
if ($p.ExitCode -eq 0) {
    Write-Host "[+] Build SUCCESSFUL -> $Output" -ForegroundColor Green
    exit 0
}

Write-Host "[-] Build FAILED with exit code $($p.ExitCode)" -ForegroundColor Red
exit $p.ExitCode
