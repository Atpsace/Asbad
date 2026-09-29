#pragma once
#include <windows.h>
#include <cstdint>
#include <cstdio>

namespace matchio {
    enum class mouse_button {
        left = 0,
        right = 1,
        middle = 2
    };

    // Correct IOCTL contract for 204.sys (MatchIo driver - WinIo_Keybd lineage)
    // 204.sys is a kernel input injection driver targeting kbdclass/mouclass.
    // MUST call IOCTL_INIT (0x80102214) immediately upon opening device handle
    // to resolve driver hook pointers; failing to do so causes a NULL dereference BSOD.
    constexpr DWORD IOCTL_MATCHIO_INIT          = 0x80102214;
    constexpr DWORD IOCTL_MATCHIO_KEY_DOWN      = 0x80102110;
    constexpr DWORD IOCTL_MATCHIO_KEY_UP        = 0x80102130;
    constexpr DWORD IOCTL_MATCHIO_MOUSE_MOVE    = 0x80102150;
    constexpr DWORD IOCTL_MATCHIO_LBUTTON_DOWN  = 0x80102170;
    constexpr DWORD IOCTL_MATCHIO_LBUTTON_UP    = 0x80102190;
    constexpr DWORD IOCTL_MATCHIO_RBUTTON_DOWN  = 0x801021B0;
    constexpr DWORD IOCTL_MATCHIO_RBUTTON_UP    = 0x801021D0;
    constexpr DWORD IOCTL_MATCHIO_MBUTTON_DOWN  = 0x801021F0;
    constexpr DWORD IOCTL_MATCHIO_MBUTTON_UP    = 0x80102210;

    #pragma pack(push, 1)
    struct KEY_REQUEST {
        DWORD scanCode;
    };

    struct MOUSE_REQUEST {
        int deltaX;
        int deltaY;
    };
    #pragma pack(pop)

    class InputDevice {
        HANDLE hDevice = INVALID_HANDLE_VALUE;
        DWORD lastError = 0;

        bool EnsureServiceLoaded(const wchar_t* driverPath) {
            SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
            if (!hSCM) return false;

            const wchar_t* serviceName = L"MatchIo";
            SC_HANDLE hService = OpenServiceW(hSCM, serviceName, SERVICE_START | SERVICE_CHANGE_CONFIG);

            if (!hService) {
                hService = CreateServiceW(
                    hSCM, serviceName, serviceName,
                    SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_CHANGE_CONFIG,
                    SERVICE_KERNEL_DRIVER,
                    SERVICE_DEMAND_START,
                    SERVICE_ERROR_NORMAL,
                    driverPath, NULL, NULL, NULL, NULL, NULL
                );
            } else {
                // Update binary path in case driver moved
                ChangeServiceConfigW(
                    hService, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
                    SERVICE_ERROR_NORMAL, driverPath, NULL, NULL, NULL, NULL, NULL, NULL
                );
            }

            if (hService) {
                StartServiceW(hService, 0, NULL);
                CloseServiceHandle(hService);
            }
            CloseServiceHandle(hSCM);
            return true;
        }

    public:
        InputDevice() = default;
        ~InputDevice() { close(); }

        void close() {
            if (hDevice != INVALID_HANDLE_VALUE) {
                CloseHandle(hDevice);
                hDevice = INVALID_HANDLE_VALUE;
            }
        }

        bool init(const wchar_t* path) {
            close();
            lastError = 0;

            if (path && GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES) {
                EnsureServiceLoaded(path);
            }

            // 204.sys registers symbolic link \DosDevices\MatchIo
            hDevice = CreateFileW(
                L"\\\\.\\MatchIo",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );

            if (hDevice == INVALID_HANDLE_VALUE) {
                lastError = GetLastError();
                return false;
            }

            // CRITICAL SAFETY STEP: Send Init IOCTL 0x80102214 to populate kbdclass/mouclass pointers.
            // If skipped, subsequent mouse/key IOCTL calls will dereference uninitialized kernel memory -> BSOD!
            DWORD bytesReturned = 0;
            if (!DeviceIoControl(hDevice, IOCTL_MATCHIO_INIT, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
                lastError = GetLastError();
                close();
                return false;
            }

            return true;
        }

        bool is_open() const { return hDevice != INVALID_HANDLE_VALUE; }
        DWORD last_error() const { return lastError; }
        HANDLE native_handle() const { return hDevice; }

        bool mouse_move_relative(int x, int y) {
            if (!is_open()) return false;

            MOUSE_REQUEST req = { x, y };
            DWORD returned = 0;
            return DeviceIoControl(hDevice, IOCTL_MATCHIO_MOUSE_MOVE, &req, sizeof(req), NULL, 0, &returned, NULL) != FALSE;
        }

        bool mouse_click(mouse_button btn) {
            if (!is_open()) return false;

            DWORD downIoctl = IOCTL_MATCHIO_LBUTTON_DOWN;
            DWORD upIoctl   = IOCTL_MATCHIO_LBUTTON_UP;
            if (btn == mouse_button::right) {
                downIoctl = IOCTL_MATCHIO_RBUTTON_DOWN;
                upIoctl   = IOCTL_MATCHIO_RBUTTON_UP;
            } else if (btn == mouse_button::middle) {
                downIoctl = IOCTL_MATCHIO_MBUTTON_DOWN;
                upIoctl   = IOCTL_MATCHIO_MBUTTON_UP;
            }

            MOUSE_REQUEST req = { 0, 0 };
            DWORD returned = 0;
            bool res1 = DeviceIoControl(hDevice, downIoctl, &req, sizeof(req), NULL, 0, &returned, NULL) != FALSE;
            Sleep(10);
            bool res2 = DeviceIoControl(hDevice, upIoctl, &req, sizeof(req), NULL, 0, &returned, NULL) != FALSE;
            return res1 && res2;
        }

        bool key_down(uint16_t scanCode) {
            if (!is_open()) return false;

            KEY_REQUEST req = { (DWORD)scanCode };
            DWORD returned = 0;
            return DeviceIoControl(hDevice, IOCTL_MATCHIO_KEY_DOWN, &req, sizeof(req), NULL, 0, &returned, NULL) != FALSE;
        }

        bool key_up(uint16_t scanCode) {
            if (!is_open()) return false;

            KEY_REQUEST req = { (DWORD)scanCode };
            DWORD returned = 0;
            return DeviceIoControl(hDevice, IOCTL_MATCHIO_KEY_UP, &req, sizeof(req), NULL, 0, &returned, NULL) != FALSE;
        }

        bool key_tap(uint16_t scanCode) {
            if (!is_open()) return false;
            bool d = key_down(scanCode);
            Sleep(10);
            bool u = key_up(scanCode);
            return d && u;
        }
    };

    inline InputDevice g_InputDevice;
}

