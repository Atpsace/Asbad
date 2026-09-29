#include "aimbot.h"
#include "../core/offsets.h"
#include <cmath>
#include <algorithm>

namespace Features {

AimbotConfig Aimbot::s_config;
bool Aimbot::s_initialized = false;
int Aimbot::s_lockedTarget = -1;
float Aimbot::s_remainderX = 0.0f;
float Aimbot::s_remainderY = 0.0f;

// Driver input stubs - will be wired to actual driver
static void MouseMove(long dx, long dy) {
    if (dx == 0 && dy == 0) return;
    mouse_event(MOUSEEVENTF_MOVE, (DWORD)dx, (DWORD)dy, 0, 0);
}

void Aimbot::Initialize() {
    s_initialized = true;
    s_lockedTarget = -1;
    s_remainderX = 0.0f;
    s_remainderY = 0.0f;
}

void Aimbot::Shutdown() {
    s_initialized = false;
}

void Aimbot::SetConfig(const AimbotConfig& cfg) {
    s_config = cfg;
}

AimbotConfig& Aimbot::GetConfig() {
    return s_config;
}

bool Aimbot::IsActive() {
    return s_initialized && s_config.enabled && ShouldAim();
}

int Aimbot::GetLockedTarget() {
    return s_lockedTarget;
}

bool Aimbot::ShouldAim() {
    if (s_config.key_mode == 2) return true;
    if (s_config.key_mode == 0) return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    int vk = s_config.aim_key > 0 ? s_config.aim_key : VK_LMENU;
    bool held = (GetAsyncKeyState(vk) & 0x8000) != 0;
    if (!held && vk == VK_MENU) {
        held = (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0 || (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
    }
    return held;
}

Vec3 Aimbot::GetBonePosition(const EntityData& ent, int hitbox) {
    int bone = hitbox == 0 ? 7 : hitbox == 1 ? 6 : hitbox == 2 ? 4 : 1;
    if (ent.hasBones && (ent.bones[bone].x || ent.bones[bone].y || ent.bones[bone].z)) {
        return ent.bones[bone];
    }
    float offset = hitbox == 1 ? -7.5f : hitbox == 2 ? -19.0f : hitbox == 3 ? -35.0f : 0.0f;
    return {ent.headPos.x, ent.headPos.y, ent.headPos.z + offset};
}

void Aimbot::Update(float deltaTime, int screenW, int screenH) {
    if (!s_initialized || !s_config.enabled) return;
    
    bool keyHeld = ShouldAim();
    if (!keyHeld) {
        s_remainderX = 0.0f;
        s_remainderY = 0.0f;
        s_lockedTarget = -1;
        return;
    }
    
    ProcessTarget(deltaTime, screenW, screenH);
}

void Aimbot::ProcessTarget(float deltaTime, int screenW, int screenH) {
    const auto& entities = EntityScanner::GetEntities();
    Matrix4x4 vm = EntityScanner::GetViewMatrix();
    Vec3 localOrigin = EntityScanner::GetLocalOrigin();
    int localTeam = EntityScanner::GetLocalTeam();
    
    float cx = screenW * 0.5f;
    float cy = screenH * 0.5f;
    float fovPx = s_config.fov * (screenH / 90.0f);
    
    float bestDist = 999999.0f;
    Vec2 bestScr = {cx, cy};
    const EntityData* bestEnt = nullptr;
    
    for (const auto& ent : entities) {
        if (!ent.valid || ent.health <= 0) continue;
        if (localTeam && ent.team == localTeam) continue;
        if (s_config.visible_check && !ent.spotted && !ent.spottedByLocal) continue;
        if (s_config.min_damage > 0 && ent.health < s_config.min_damage) continue;
        
        Vec3 aimWorld = GetBonePosition(ent, s_config.hitbox);
        Vec2 aimScr;
        if (!ESP::WorldToScreen(aimWorld, vm, aimScr, screenW, screenH)) continue;
        
        float dx = aimScr.x - cx;
        float dy = aimScr.y - cy;
        float dist2D = std::sqrt(dx * dx + dy * dy);
        if (dist2D > fovPx) continue;
        
        float score = s_config.priority == 1 ? localOrigin.Distance(ent.origin) : s_config.priority == 2 ? (float)ent.health : dist2D;
        if (score < bestDist) {
            bestDist = score;
            bestScr = aimScr;
            bestEnt = &ent;
        }
    }
    
    if (!bestEnt) {
        s_remainderX = 0.0f;
        s_remainderY = 0.0f;
        s_lockedTarget = -1;
        return;
    }
    
    s_lockedTarget = bestEnt->entitySlot;
    
    float movX = bestScr.x - cx;
    float movY = bestScr.y - cy;
    float targetDist = std::sqrt(movX * movX + movY * movY);
    if (targetDist < 2.0f) return;
    
    float alpha = 1.0f;
    if (s_config.smoothing && s_config.smooth_strength > 0.1f) {
        float rate = 32.0f / s_config.smooth_strength;
        if (s_config.smooth_type == 1) {
            float norm = std::clamp(targetDist / fovPx, 0.05f, 1.0f);
            rate *= (0.45f + 0.85f * (1.0f - std::pow(1.0f - norm, 2.2f)));
        } else if (s_config.smooth_type == 2) {
            float norm = std::clamp(targetDist / fovPx, 0.02f, 1.0f);
            rate *= (0.35f + std::pow(norm, 1.3f) * 1.25f);
        }
        alpha = 1.0f - std::exp(-rate * deltaTime);
        alpha = std::clamp(alpha, 0.02f, 0.85f);
    }
    
    float sens = 1.0f;
    float focalLen = screenH * 0.666667f;
    float yawDeg = std::atan2(movX, focalLen) * (180.0f / 3.14159265f);
    float pitchDeg = std::atan2(movY, focalLen) * (180.0f / 3.14159265f);
    float countsX = yawDeg / (0.022f * sens);
    float countsY = pitchDeg / (0.022f * sens);
    
    float stepX = (countsX * alpha) + s_remainderX;
    float stepY = (countsY * alpha) + s_remainderY;
    
    stepX = std::clamp(stepX, -32.0f, 32.0f);
    stepY = std::clamp(stepY, -32.0f, 32.0f);
    
    long dx = (long)std::lroundf(stepX);
    long dy = (long)std::lroundf(stepY);
    s_remainderX = stepX - (float)dx;
    s_remainderY = stepY - (float)dy;
    
    if (dx || dy) MouseMove(dx, dy);
}

} // namespace Features
