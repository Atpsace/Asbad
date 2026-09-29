// driver_client.h — Talk to the mapped driver
#pragma once

#include <windows.h>

#define IOCTL_READ_MEMORY           0x80102040
#define IOCTL_WRITE_MEMORY          0x80102044
#define IOCTL_GET_PID               0x80102048
#define IOCTL_GET_BASE              0x8010204C
#define IOCTL_HIDE_PROCESS          0x80102230
#define IOCTL_HIDE_PROCESS_REDIRECT 0x80102234
#define IOCTL_CLEAN_HANDLE_TABLE    0x80102238
#define IOCTL_HIDE_THREAD           0x8010223C

typedef struct {
    ULONG ProcessId;
    ULONG_PTR Address;
    SIZE_T Size;
    ULONG_PTR Buffer;
} MEMORY_REQUEST;

typedef struct {
    DWORD ProcessId;
    DWORD DecoyProcessId;
} HIDE_PROCESS_REDIRECT_REQUEST;

typedef struct {
    DWORD ProcessId;
    DWORD TargetId;
} CLEAN_HANDLE_REQUEST;

typedef struct {
    DWORD ProcessId;
    DWORD ThreadId;
} HIDE_THREAD_REQUEST;

class DriverClient {
    HANDLE hDriver = INVALID_HANDLE_VALUE;

    bool IsConnected() const {
        return hDriver != INVALID_HANDLE_VALUE;
    }

public:
    DriverClient() = default;
    ~DriverClient() { Disconnect(); }
    DriverClient(const DriverClient&) = delete;
    DriverClient& operator=(const DriverClient&) = delete;

    bool Connect() {
        if (IsConnected())
            return true;

        hDriver = CreateFileA("\\\\.\\CheatDriver", GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING, 0, NULL);
        return IsConnected();
    }

    void Disconnect() {
        if (IsConnected()) {
            CloseHandle(hDriver);
            hDriver = INVALID_HANDLE_VALUE;
        }
    }

    HANDLE GetHandle() const { return hDriver; }

    bool Read(ULONG pid, ULONG_PTR addr, void* buf, SIZE_T size) {
        if (!IsConnected() || pid == 0 || addr == 0 || buf == nullptr || size == 0)
            return false;
        MEMORY_REQUEST req = {pid, addr, size, (ULONG_PTR)buf};
        DWORD returned = 0;
        return DeviceIoControl(hDriver, IOCTL_READ_MEMORY, &req, sizeof(req),
                               buf, (DWORD)size, &returned, NULL) && returned == size;
    }

    bool Write(ULONG pid, ULONG_PTR addr, void* buf, SIZE_T size) {
        if (!IsConnected() || pid == 0 || addr == 0 || buf == nullptr || size == 0)
            return false;
        MEMORY_REQUEST req = {pid, addr, size, (ULONG_PTR)buf};
        DWORD returned = 0;
        return DeviceIoControl(hDriver, IOCTL_WRITE_MEMORY, &req, sizeof(req), NULL,
                               0, &returned, NULL); 
    }
    
    ULONG GetPid(const wchar_t* processName) {
        if (hDriver == INVALID_HANDLE_VALUE) return 0;
        WCHAR name[64] = {0};
        wcscpy_s(name, processName);
        DWORD returned = 0;
        ULONG pidOut = 0;
        if (!DeviceIoControl(hDriver, IOCTL_GET_PID, name, sizeof(name),
                             &pidOut, sizeof(pidOut), &returned, NULL))
            return 0;
        return pidOut;
    }
    
    ULONG_PTR GetBase(ULONG pid, const wchar_t* moduleName) {
        if (hDriver == INVALID_HANDLE_VALUE) return 0;
        struct { ULONG ProcessId; WCHAR ModuleName[64]; ULONG_PTR Address; } req = {pid, {0}, 0};
        wcscpy_s(req.ModuleName, moduleName);
        DWORD returned = 0;
        DeviceIoControl(hDriver, IOCTL_GET_BASE, &req, sizeof(req), &req, sizeof(req), &returned, NULL);
        return req.Address;
    }

    bool HideProcessRedirect(DWORD pid, DWORD decoyPid = 4) {
        if (hDriver == INVALID_HANDLE_VALUE) return false;
        HIDE_PROCESS_REDIRECT_REQUEST req = { pid, decoyPid };
        DWORD returned = 0;
        return DeviceIoControl(hDriver, IOCTL_HIDE_PROCESS_REDIRECT, &req, sizeof(req), NULL, 0, &returned, NULL);
    }

    bool CleanHandleTable(DWORD pid) {
        if (hDriver == INVALID_HANDLE_VALUE) return false;
        CLEAN_HANDLE_REQUEST req = { pid, 0 };
        DWORD returned = 0;
        return DeviceIoControl(hDriver, IOCTL_CLEAN_HANDLE_TABLE, &req, sizeof(req), NULL, 0, &returned, NULL);
    }

    bool HideThread(DWORD pid, DWORD threadId) {
        if (hDriver == INVALID_HANDLE_VALUE) return false;
        HIDE_THREAD_REQUEST req = { pid, threadId };
        DWORD returned = 0;
        return DeviceIoControl(hDriver, IOCTL_HIDE_THREAD, &req, sizeof(req), NULL, 0, &returned, NULL);
    }
};

