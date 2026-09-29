#include "batch_memory.h"
#include <algorithm>

namespace Core {

bool BatchMemory::BatchRead(HANDLE process, const std::vector<ReadRequest>& requests) {
    if (!process || requests.empty()) return false;
    
    bool allSuccess = true;
    for (const auto& req : requests) {
        SIZE_T read = 0;
        if (!ReadProcessMemory(process, (LPCVOID)req.address, req.buffer, req.size, &read) || read != req.size) {
            allSuccess = false;
        }
    }
    return allSuccess;
}

bool BatchMemory::ReadChunk(HANDLE process, uintptr_t base, size_t size, void* buffer) {
    if (!process || !buffer || size == 0) return false;
    SIZE_T read = 0;
    return ReadProcessMemory(process, (LPCVOID)base, buffer, size, &read) && read == size;
}

bool BatchMemory::ReadEntityBatch(HANDLE process, uintptr_t pawn, void* buffer, size_t bufferSize) {
    // Read entire entity in one call - health, team, origin, velocity, armor, etc
    // Typical range: 0x330 to 0x2000 (~7KB)
    constexpr uintptr_t kStart = 0x330;
    constexpr size_t kSize = 0x1CD0; // 7KB covers all common offsets
    
    if (bufferSize < kSize) return false;
    return ReadChunk(process, pawn + kStart, kSize, buffer);
}

bool BatchMemory::ReadControllerBlock(HANDLE process, uintptr_t listEntry, void* buffer, size_t count) {
    // Read 64 controller slots in one call (64 * 0x78 = 7680 bytes)
    size_t blockSize = count * 0x78;
    return ReadChunk(process, listEntry, blockSize, buffer);
}

} // namespace Core
