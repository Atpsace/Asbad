#pragma once
#include "../core/types.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include <vector>

namespace Features {

struct EntityData {
    bool valid = false;
    bool hasBomb = false;
    bool spotted = false;
    bool spottedByLocal = false;
    bool hasBones = false;
    uintptr_t pawn = 0;
    uintptr_t boneArrayAddress = 0;
    int health = 0;
    int armor = 0;
    int ammo = -1;
    int team = 0;
    int entitySlot = 0;
    int shotsFired = 0;
    char name[64] = {};
    char weaponName[24] = {};
    Vec3 origin;
    Vec3 velocity;
    Vec3 headPos;
    Vec3 bones[32] = {};
    Vec2 eyeAngles;
    float lastSoundTime = 0.0f;
};

struct RenderContext {
    const Matrix4x4* viewMatrix;
    Vec3 localOrigin;
    int localTeam;
    int screenW;
    int screenH;
    void* drawList;
};

class ESP {
public:
    static void Initialize();
    static void Shutdown();
    
    static void RenderFrame(const RenderContext& ctx, const std::vector<EntityData>& entities);
    
    // Individual render components
    static void DrawBox(const RenderContext& ctx, const EntityData& ent, const Vec2& topLeft, const Vec2& bottomRight);
    static void DrawSkeleton(const RenderContext& ctx, const EntityData& ent);
    static void DrawHealthBar(const RenderContext& ctx, const EntityData& ent, const Vec2& topLeft, float boxHeight);
    static void DrawArmorBar(const RenderContext& ctx, const EntityData& ent, const Vec2& topLeft, const Vec2& bottomRight);
    static void DrawAmmoBar(const RenderContext& ctx, const EntityData& ent, const Vec2& topLeft, const Vec2& bottomRight);
    static void DrawNameDistance(const RenderContext& ctx, const EntityData& ent, const Vec2& topCenter, float distance);
    static void DrawWeapon(const RenderContext& ctx, const EntityData& ent, const Vec2& bottomCenter);
    static void DrawHeadDot(const RenderContext& ctx, const Vec2& headScreen, float boxHeight);
    static void DrawSnapline(const RenderContext& ctx, const Vec2& entityScreen);
    static void DrawEyeRay(const RenderContext& ctx, const EntityData& ent, const Vec2& headScreen);
    static void DrawFlags(const RenderContext& ctx, const EntityData& ent, const Vec2& flagStart);
    
    // Utilities
    static bool WorldToScreen(const Vec3& world, const Matrix4x4& vm, Vec2& screen, int screenW, int screenH);
    static void UpdateW2SCache(const Matrix4x4& vm, int screenW, int screenH);
    
private:
    static bool s_initialized;
};

} // namespace Features
