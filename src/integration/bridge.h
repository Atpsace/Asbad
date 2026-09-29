#pragma once
#include "../features/entity_scanner.h"
#include "../features/esp.h"
#include "../renderer/draw.h"
#include "../config/config.h"
#include <windows.h>

namespace Integration {

class Bridge {
public:
    static bool Initialize(HANDLE process, uintptr_t clientBase);
    static void Shutdown();
    
    static void Update();
    static void Render(void* drawList, int screenW, int screenH);
    
    static bool IsReady();
    static float GetScanTimeMs();
    static int GetEntityCount();
    
private:
    static bool s_initialized;
};

} // namespace Integration
