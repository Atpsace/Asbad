#include "esp.h"
#include "../config/config.h"
#include <immintrin.h>
#include <cmath>
#include <algorithm>

namespace Features {

bool ESP::s_initialized = false;

// W2S cache for SIMD optimization
struct W2SCache {
    __m128 r0_x, r0_y, r0_z, r0_w;
    __m128 r1_x, r1_y, r1_z, r1_w;
    __m128 r3_x, r3_y, r3_z, r3_w;
    float f_r0_x, f_r0_y, f_r0_z, f_r0_w;
    float f_r1_x, f_r1_y, f_r1_z, f_r1_w;
    float f_r3_x, f_r3_y, f_r3_z, f_r3_w;
    float halfW, halfH;
};
static W2SCache g_cache;

void ESP::Initialize() {
    s_initialized = true;
}

void ESP::Shutdown() {
    s_initialized = false;
}

void ESP::UpdateW2SCache(const Matrix4x4& vm, int screenW, int screenH) {
    g_cache.halfW = screenW * 0.5f;
    g_cache.halfH = screenH * 0.5f;
    float hw = g_cache.halfW;
    float nh = -g_cache.halfH;

    g_cache.f_r0_x = vm.m[0][0] * hw;
    g_cache.f_r0_y = vm.m[0][1] * hw;
    g_cache.f_r0_z = vm.m[0][2] * hw;
    g_cache.f_r0_w = vm.m[0][3] * hw;

    g_cache.f_r1_x = vm.m[1][0] * nh;
    g_cache.f_r1_y = vm.m[1][1] * nh;
    g_cache.f_r1_z = vm.m[1][2] * nh;
    g_cache.f_r1_w = vm.m[1][3] * nh;

    g_cache.f_r3_x = vm.m[3][0];
    g_cache.f_r3_y = vm.m[3][1];
    g_cache.f_r3_z = vm.m[3][2];
    g_cache.f_r3_w = vm.m[3][3];

    g_cache.r0_x = _mm_set1_ps(g_cache.f_r0_x);
    g_cache.r0_y = _mm_set1_ps(g_cache.f_r0_y);
    g_cache.r0_z = _mm_set1_ps(g_cache.f_r0_z);
    g_cache.r0_w = _mm_set1_ps(g_cache.f_r0_w);

    g_cache.r1_x = _mm_set1_ps(g_cache.f_r1_x);
    g_cache.r1_y = _mm_set1_ps(g_cache.f_r1_y);
    g_cache.r1_z = _mm_set1_ps(g_cache.f_r1_z);
    g_cache.r1_w = _mm_set1_ps(g_cache.f_r1_w);

    g_cache.r3_x = _mm_set1_ps(g_cache.f_r3_x);
    g_cache.r3_y = _mm_set1_ps(g_cache.f_r3_y);
    g_cache.r3_z = _mm_set1_ps(g_cache.f_r3_z);
    g_cache.r3_w = _mm_set1_ps(g_cache.f_r3_w);
}

bool ESP::WorldToScreen(const Vec3& world, const Matrix4x4& vm, Vec2& screen, int screenW, int screenH) {
    float w = g_cache.f_r3_x * world.x + g_cache.f_r3_y * world.y + g_cache.f_r3_z * world.z + g_cache.f_r3_w;
    if (w < 0.001f) return false;
    
    float invW = 1.0f / w;
    float x = g_cache.f_r0_x * world.x + g_cache.f_r0_y * world.y + g_cache.f_r0_z * world.z + g_cache.f_r0_w;
    float y = g_cache.f_r1_x * world.x + g_cache.f_r1_y * world.y + g_cache.f_r1_z * world.z + g_cache.f_r1_w;
    
    screen.x = g_cache.halfW + x * invW;
    screen.y = g_cache.halfH + y * invW;
    return true;
}

void ESP::RenderFrame(const RenderContext& ctx, const std::vector<EntityData>& entities) {
    if (!s_initialized) return;
    
    UpdateW2SCache(*ctx.viewMatrix, ctx.screenW, ctx.screenH);
    
    extern Config::Settings g_settings;
    
    for (const auto& ent : entities) {
        if (!ent.valid || ent.health <= 0) continue;
        if (g_settings.teamCheck && ctx.localTeam && ent.team == ctx.localTeam) continue;
        
        Vec2 originScr, headScr;
        if (!WorldToScreen(ent.origin, *ctx.viewMatrix, originScr, ctx.screenW, ctx.screenH)) continue;
        if (!WorldToScreen(ent.headPos, *ctx.viewMatrix, headScr, ctx.screenW, ctx.screenH)) continue;
        
        float boxH = std::abs(headScr.y - originScr.y) * 1.12f;
        float boxW = boxH * 0.55f;
        Vec2 boxTL = {originScr.x - boxW * 0.5f, headScr.y - boxH * 0.08f};
        Vec2 boxBR = {originScr.x + boxW * 0.5f, originScr.y + boxH * 0.04f};
        
        float distance = ctx.localOrigin.Distance(ent.origin) * 0.0254f;
        
        if (g_settings.boxes) DrawBox(ctx, ent, boxTL, boxBR);
        if (g_settings.skeleton) DrawSkeleton(ctx, ent);
        if (g_settings.health) DrawHealthBar(ctx, ent, boxTL, boxH);
        if (g_settings.armor && ent.armor > 0) DrawArmorBar(ctx, ent, boxTL, boxBR);
        if (g_settings.names || g_settings.distance) DrawNameDistance(ctx, ent, {originScr.x, boxTL.y}, distance);
        if (g_settings.weapon && ent.weaponName[0]) DrawWeapon(ctx, ent, {originScr.x, boxBR.y});
        if (g_settings.headDot) DrawHeadDot(ctx, headScr, boxH);
        if (g_settings.snaplines) DrawSnapline(ctx, originScr);
        if (g_settings.eyeRay) DrawEyeRay(ctx, ent, headScr);
    }
}

void ESP::DrawBox(const RenderContext& ctx, const EntityData& ent, const Vec2& tl, const Vec2& br) {
    // Stub - requires ImGui integration
}

void ESP::DrawSkeleton(const RenderContext& ctx, const EntityData& ent) {
    if (!ent.hasBones) return;
    // Stub - requires ImGui integration
}

void ESP::DrawHealthBar(const RenderContext& ctx, const EntityData& ent, const Vec2& tl, float boxH) {
    // Stub - requires ImGui integration
}

void ESP::DrawArmorBar(const RenderContext& ctx, const EntityData& ent, const Vec2& tl, const Vec2& br) {
    // Stub - requires ImGui integration
}

void ESP::DrawAmmoBar(const RenderContext& ctx, const EntityData& ent, const Vec2& tl, const Vec2& br) {
    // Stub - requires ImGui integration
}

void ESP::DrawNameDistance(const RenderContext& ctx, const EntityData& ent, const Vec2& topCenter, float distance) {
    // Stub - requires ImGui integration
}

void ESP::DrawWeapon(const RenderContext& ctx, const EntityData& ent, const Vec2& bottomCenter) {
    // Stub - requires ImGui integration
}

void ESP::DrawHeadDot(const RenderContext& ctx, const Vec2& headScreen, float boxHeight) {
    // Stub - requires ImGui integration
}

void ESP::DrawSnapline(const RenderContext& ctx, const Vec2& entityScreen) {
    // Stub - requires ImGui integration
}

void ESP::DrawEyeRay(const RenderContext& ctx, const EntityData& ent, const Vec2& headScreen) {
    // Stub - requires ImGui integration
}

void ESP::DrawFlags(const RenderContext& ctx, const EntityData& ent, const Vec2& flagStart) {
    // Stub - requires ImGui integration
}

} // namespace Features
