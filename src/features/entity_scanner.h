#pragma once
#include "../core/types.h"
#include "../core/batch_memory.h"
#include "../core/offsets.h"
#include "esp.h"
#include <windows.h>
#include <vector>
#include <atomic>
#include <thread>

namespace Features {

class EntityScanner {
public:
    static void Initialize(HANDLE process, uintptr_t clientBase);
    static void Shutdown();
    
    static void StartScanThread();
    static void StopScanThread();
    
    // Get latest entity snapshot (triple buffered, lock-free)
    static const std::vector<EntityData>& GetEntities();
    static Vec3 GetLocalOrigin();
    static int GetLocalTeam();
    static Matrix4x4 GetViewMatrix();
    
    // Performance stats
    static float GetScanTimeMs();
    static int GetEntityCount();
    
private:
    static void ScanThreadLoop();
    static void ScanEntitiesOptimized();
    
    static HANDLE s_process;
    static uintptr_t s_clientBase;
    static std::thread s_scanThread;
    static std::atomic<bool> s_running;
    
    // Triple buffered snapshots
    static std::vector<EntityData> s_snapshots[3];
    static std::atomic<int> s_readIndex;
    static std::atomic<int> s_writeIndex;
    
    static Core::CachedEntityData s_cache[65];
    static Vec3 s_localOrigin;
    static int s_localTeam;
    static Matrix4x4 s_viewMatrix;
    static std::atomic<float> s_scanTimeMs;
};

} // namespace Features
