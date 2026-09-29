#pragma once
#include <windows.h>
#include <cstdint>
#include <vector>

namespace Core {

// Batched memory reading for massive perf boost
class BatchMemory {
public:
    struct ReadRequest {
        uintptr_t address;
        void* buffer;
        size_t size;
    };
    
    static bool BatchRead(HANDLE process, const std::vector<ReadRequest>& requests);
    static bool ReadChunk(HANDLE process, uintptr_t base, size_t size, void* buffer);
    
    // Fast entity batch read - single RPM call
    static bool ReadEntityBatch(HANDLE process, uintptr_t pawn, void* buffer, size_t bufferSize);
    
    // Read entire controller block
    static bool ReadControllerBlock(HANDLE process, uintptr_t listEntry, void* buffer, size_t count);
};

// Entity data cache - minimize RPM calls
struct CachedEntityData {
    uintptr_t pawn = 0;
    uintptr_t controller = 0;
    uintptr_t boneArray = 0;
    uintptr_t gameSceneNode = 0;
    uint32_t pawnHandle = 0;
    char name[64] = {};
    char weaponName[24] = {};
    uint8_t team = 0;
    uint64_t lastUpdate = 0;
    bool valid = false;
};

} // namespace Core
