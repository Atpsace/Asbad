#include "bridge.h"

namespace Integration {

bool Bridge::s_initialized = false;

bool Bridge::Initialize(HANDLE process, uintptr_t clientBase) {
    if (s_initialized) return true;
    
    Features::EntityScanner::Initialize(process, clientBase);
    Features::EntityScanner::StartScanThread();
    Features::ESP::Initialize();
    
    s_initialized = true;
    return true;
}

void Bridge::Shutdown() {
    if (!s_initialized) return;
    
    Features::EntityScanner::Shutdown();
    Features::ESP::Shutdown();
    
    s_initialized = false;
}

void Bridge::Update() {
    // Future: add input handling, config updates here
}

void Bridge::Render(void* drawList, int screenW, int screenH) {
    if (!s_initialized) return;
    
    const auto& entities = Features::EntityScanner::GetEntities();
    Matrix4x4 vm = Features::EntityScanner::GetViewMatrix();
    Vec3 localOrigin = Features::EntityScanner::GetLocalOrigin();
    int localTeam = Features::EntityScanner::GetLocalTeam();
    
    Features::RenderContext ctx;
    ctx.viewMatrix = &vm;
    ctx.localOrigin = localOrigin;
    ctx.localTeam = localTeam;
    ctx.screenW = screenW;
    ctx.screenH = screenH;
    ctx.drawList = drawList;
    
    Features::ESP::RenderFrame(ctx, entities);
}

bool Bridge::IsReady() {
    return s_initialized;
}

float Bridge::GetScanTimeMs() {
    return Features::EntityScanner::GetScanTimeMs();
}

int Bridge::GetEntityCount() {
    return Features::EntityScanner::GetEntityCount();
}

} // namespace Integration
