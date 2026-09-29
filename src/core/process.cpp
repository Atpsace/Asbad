#include "process.h"
#include <tlhelp32.h>

namespace Core {

ProcessInfo ProcessManager::s_process;

bool ProcessManager::Attach(const wchar_t* processName) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return false;
    
    PROCESSENTRY32W pe = {sizeof(PROCESSENTRY32W)};
    bool found = false;
    
    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, processName) == 0) {
                s_process.pid = pe.th32ProcessID;
                found = true;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    
    CloseHandle(snap);
    if (!found) return false;
    
    s_process.handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, s_process.pid);
    if (!s_process.handle) return false;
    
    s_process.clientBase = GetModuleBase(L"client.dll");
    s_process.valid = (s_process.clientBase != 0);
    
    return s_process.valid;
}

void ProcessManager::Detach() {
    if (s_process.handle) {
        CloseHandle(s_process.handle);
        s_process = ProcessInfo{};
    }
}

bool ProcessManager::IsValid() {
    if (!s_process.valid || !s_process.handle) return false;
    DWORD exitCode = 0;
    if (!GetExitCodeProcess(s_process.handle, &exitCode)) return false;
    return exitCode == STILL_ACTIVE;
}

ProcessInfo& ProcessManager::Get() {
    return s_process;
}

bool ProcessManager::ReadRaw(uintptr_t address, void* buffer, size_t size) {
    SIZE_T read = 0;
    return ReadProcessMemory(s_process.handle, (void*)address, buffer, size, &read) && read == size;
}

uintptr_t ProcessManager::GetModuleBase(const wchar_t* moduleName) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, s_process.pid);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    
    MODULEENTRY32W me = {sizeof(MODULEENTRY32W)};
    uintptr_t base = 0;
    
    if (Module32FirstW(snap, &me)) {
        do {
            if (_wcsicmp(me.szModule, moduleName) == 0) {
                base = (uintptr_t)me.modBaseAddr;
                break;
            }
        } while (Module32NextW(snap, &me));
    }
    
    CloseHandle(snap);
    return base;
}

} // namespace Core
