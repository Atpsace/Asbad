@echo off
title Spring ESP Launcher
color 0A

echo.
echo  ============================
echo   SPRING CS2 ESP  ^|  LOADER
echo  ============================
echo.

:: Check if running as admin
net session >nul 2>&1
if %errorlevel% NEQ 0 (
    echo  [!] Requesting administrator privileges...
    powershell -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

:: Clean up any lingering MatchIo kernel service to prevent BSODs
set SVC_NAME=MatchIo
sc query %SVC_NAME% >nul 2>&1
if %errorlevel% EQU 0 (
    echo  [*] Cleaning up legacy MatchIo kernel service...
    sc stop %SVC_NAME% >nul 2>&1
    sc delete %SVC_NAME% >nul 2>&1
    timeout /t 1 /nobreak >nul
    echo  [+] Legacy MatchIo service removed safely.
)

echo  [*] Mode: Safe Usermode (mouse_event / SendInput)
echo  [*] Kernel driver load disabled (prevents PatchGuard / BugCheck BSOD)
echo.
echo  [*] Launching Spring ESP...
echo.

:: Launch ESP from the same directory
start "" "%~dp0esp_fast.exe"

echo  [+] ESP launched successfully.
echo.
echo  This window will close in 3 seconds...
timeout /t 3 /nobreak >nul
