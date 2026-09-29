#include "entity_scanner.h"
#include <chrono>
#include <algorithm>

using namespace std::chrono;

namespace Features {

HANDLE EntityScanner::s_process = nullptr;
uintptr_t EntityScanner::s_clientBase = 0;
std::thread EntityScanner::s_scanThread;
std::atomic<bool> EntityScanner::s_running{false};

std::vector<EntityData> EntityScanner::s_snapshots[3];
std::atomic<int> EntityScanner::s_readIndex{0};
std::atomic<int> EntityScanner::s_writeIndex{1};

Core::CachedEntityData EntityScanner::s_cache[65];
Vec3 EntityScanner::s_localOrigin;
int EntityScanner::s_localTeam = 0;
Matrix4x4 EntityScanner::s_viewMatrix;
std::atomic<float> EntityScanner::s_scanTimeMs{0.0f};

void EntityScanner::Initialize(HANDLE process, uintptr_t clientBase) {
    s_process = process;
    s_clientBase = clientBase;
    
    for (int i = 0; i < 3; i++) {
        s_snapshots[i].reserve(64);
    }
}

void EntityScanner::Shutdown() {
    StopScanThread();
}

void EntityScanner::StartScanThread() {
    if (s_running.load()) return;
    s_running.store(true);
    s_scanThread = std::thread(ScanThreadLoop);
}

void EntityScanner::StopScanThread() {
    s_running.store(false);
    if (s_scanThread.joinable()) {
        s_scanThread.join();
    }
}

const std::vector<EntityData>& EntityScanner::GetEntities() {
    return s_snapshots[s_readIndex.load(std::memory_order_acquire)];
}

Vec3 EntityScanner::GetLocalOrigin() { return s_localOrigin; }
int EntityScanner::GetLocalTeam() { return s_localTeam; }
Matrix4x4 EntityScanner::GetViewMatrix() { return s_viewMatrix; }
float EntityScanner::GetScanTimeMs() { return s_scanTimeMs.load(); }

int EntityScanner::GetEntityCount() {
    return (int)GetEntities().size();
}

void EntityScanner::ScanThreadLoop() {
    while (s_running.load()) {
        auto start = high_resolution_clock::now();
        
        ScanEntitiesOptimized();
        
        auto end = high_resolution_clock::now();
        float elapsed = duration<float, std::milli>(end - start).count();
        s_scanTimeMs.store(elapsed);
        
        // Swap buffers
        int oldWrite = s_writeIndex.load();
        int newWrite = 0;
        for (int i = 0; i < 3; i++) {
            if (i != oldWrite && i != s_readIndex.load()) {
                newWrite = i;
                break;
            }
        }
        s_readIndex.store(oldWrite, std::memory_order_release);
        s_writeIndex.store(newWrite);
        
        // Target 1000Hz scan rate
        Sleep(1);
    }
}

void EntityScanner::ScanEntitiesOptimized() {
    if (!s_process || !s_clientBase) return;
    
    int writeIdx = s_writeIndex.load();
    auto& snapshot = s_snapshots[writeIdx];
    snapshot.clear();
    
    // Read view matrix (single RPM)
    Core::BatchMemory::ReadChunk(s_process, s_clientBase + Offsets::Client::dwViewMatrix, 
                                 sizeof(s_viewMatrix), &s_viewMatrix);
    
    // Read local player
    uintptr_t localPawn = 0;
    Core::BatchMemory::ReadChunk(s_process, s_clientBase + Offsets::Client::dwLocalPlayerPawn,
                                 sizeof(localPawn), &localPawn);
    
    if (localPawn) {
        // Batch read local data
        uint8_t localBuf[512];
        if (Core::BatchMemory::ReadChunk(s_process, localPawn + 0x340, sizeof(localBuf), localBuf)) {
            s_localTeam = localBuf[Offsets::Pawn::m_iTeamNum - 0x340];
            memcpy(&s_localOrigin, localBuf + (Offsets::Pawn::m_vecAbsOrigin + 0x1000 - 0x340), sizeof(s_localOrigin));
        }
    }
    
    // Read entity list
    uintptr_t entityList = 0;
    if (!Core::BatchMemory::ReadChunk(s_process, s_clientBase + Offsets::Client::dwEntityList,
                                     sizeof(entityList), &entityList) || !entityList) {
        return;
    }
    
    uintptr_t listEntry = 0;
    Core::BatchMemory::ReadChunk(s_process, entityList + 0x10, sizeof(listEntry), &listEntry);
    if (!listEntry) return;
    
    // Batch read all 64 controller slots at once (7KB single RPM call)
    uint8_t controllerBlock[64 * 0x78];
    if (!Core::BatchMemory::ReadControllerBlock(s_process, listEntry, controllerBlock, 64)) {
        return;
    }
    
    // Process entities
    for (int i = 1; i <= 64; i++) {
        uintptr_t controller = *(uintptr_t*)(controllerBlock + i * 0x78);
        if (!controller) continue;
        
        uint32_t pawnHandle = 0;
        Core::BatchMemory::ReadChunk(s_process, controller + Offsets::Controller::m_hPlayerPawn,
                                    sizeof(pawnHandle), &pawnHandle);
        if (!pawnHandle || pawnHandle == 0xFFFFFFFF) continue;
        
        // Resolve pawn
        uintptr_t pawnEntry = 0;
        Core::BatchMemory::ReadChunk(s_process, entityList + 0x10 + 8 * ((pawnHandle & 0x7FFF) >> 9),
                                    sizeof(pawnEntry), &pawnEntry);
        if (!pawnEntry) continue;
        
        uintptr_t pawn = 0;
        Core::BatchMemory::ReadChunk(s_process, pawnEntry + 0x78 * (pawnHandle & 0x1FF),
                                    sizeof(pawn), &pawn);
        if (!pawn) continue;
        
        // Batch read entire entity (single 7KB RPM call)
        uint8_t entityBuf[8192];
        if (!Core::BatchMemory::ReadEntityBatch(s_process, pawn, entityBuf, sizeof(entityBuf))) {
            continue;
        }
        
        // Extract data from buffer (zero additional RPM calls)
        constexpr uintptr_t kBase = 0x330;
        int health = *(int32_t*)(entityBuf + (Offsets::Pawn::m_iHealth - kBase));
        if (health <= 0 || health > 500) continue;
        
        uint8_t team = entityBuf[Offsets::Pawn::m_iTeamNum - kBase];
        
        EntityData ed;
        ed.valid = true;
        ed.health = health;
        ed.team = team;
        ed.pawn = pawn;
        ed.entitySlot = i;
        
        // Origin from buffer
        memcpy(&ed.origin, entityBuf + (0x14A4 - kBase), sizeof(ed.origin));
        
        // Velocity
        memcpy(&ed.velocity, entityBuf + (Offsets::Pawn::m_absVelocity - kBase), sizeof(ed.velocity));
        
        // Armor
        ed.armor = *(int32_t*)(entityBuf + (Offsets::Pawn::m_ArmorValue - kBase + 0x1000));
        
        // Head position estimate
        ed.headPos = {ed.origin.x, ed.origin.y, ed.origin.z + 65.0f};
        
        // Cache name if needed (only read once per player)
        if (s_cache[i].controller != controller || s_cache[i].name[0] == '\0') {
            char nameBuf[64] = {};
            Core::BatchMemory::ReadChunk(s_process, controller + Offsets::Controller::m_iszPlayerName,
                                        sizeof(nameBuf), nameBuf);
            strncpy_s(s_cache[i].name, nameBuf[0] ? nameBuf : "Enemy", sizeof(s_cache[i].name) - 1);
            s_cache[i].controller = controller;
        }
        strncpy_s(ed.name, s_cache[i].name, sizeof(ed.name) - 1);
        
        // Cache bones if needed
        uintptr_t gsn = *(uintptr_t*)(entityBuf + (Offsets::Pawn::m_pGameSceneNode - kBase));
        if (gsn && s_cache[i].boneArray == 0) {
            uintptr_t boneArr = 0;
            Core::BatchMemory::ReadChunk(s_process, gsn + Offsets::SceneNode::m_boneArray,
                                        sizeof(boneArr), &boneArr);
            s_cache[i].boneArray = boneArr;
        }
        ed.boneArrayAddress = s_cache[i].boneArray;
        ed.hasBones = (ed.boneArrayAddress != 0);
        
        snapshot.push_back(ed);
    }
}

} // namespace Features
