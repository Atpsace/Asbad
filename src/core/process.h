#pragma once
#include <windows.h>
#include <cstdint>

namespace Core {

struct ProcessInfo {
    HANDLE handle = nullptr;
    DWORD pid = 0;
    uintptr_t clientBase = 0;
    HWND window = nullptr;
    bool valid = false;
};

class ProcessManager {
public:
    static bool Attach(const wchar_t* processName);
    static void Detach();
    static bool IsValid();
    static ProcessInfo& Get();
    
    template<typename T>
    static bool Read(uintptr_t address, T& buffer);
    
    template<typename T>
    static bool Write(uintptr_t address, const T& buffer);
    
    static bool ReadRaw(uintptr_t address, void* buffer, size_t size);
    static uintptr_t GetModuleBase(const wchar_t* moduleName);
    
private:
    static ProcessInfo s_process;
};

template<typename T>
inline bool ProcessManager::Read(uintptr_t address, T& buffer) {
    return ReadRaw(address, &buffer, sizeof(T));
}

template<typename T>
inline bool ProcessManager::Write(uintptr_t address, const T& buffer) {
    SIZE_T written = 0;
    return WriteProcessMemory(s_process.handle, (void*)address, &buffer, sizeof(T), &written) && written == sizeof(T);
}

} // namespace Core
