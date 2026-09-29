#pragma once
#include "../core/types.h"
#include "entity_scanner.h"
#include <windows.h>

namespace Features {

struct AimbotConfig {
    bool enabled = false;
    int hitbox = 0;  // 0=head, 1=neck, 2=chest, 3=pelvis
    float fov = 5.0f;
    bool smoothing = true;
    float smooth_strength = 8.0f;
    int smooth_type = 0;  // 0=linear, 1=curve, 2=exponential
    float smooth_humanize = 5.0f;
    bool rcs_enabled = true;
    float rcs_pitch = 100.0f;
    float rcs_yaw = 100.0f;
    int rcs_start_bullet = 1;
    bool rcs_standalone = false;
    bool prediction_enabled = true;
    bool visible_check = false;
    int min_damage = 0;
    int priority = 0;  // 0=crosshair, 1=distance, 2=health
    int key_mode = 1;  // 0=lmb, 1=key, 2=always
    int aim_key = VK_LMENU;
};

class Aimbot {
public:
    static void Initialize();
    static void Shutdown();
    
    static void Update(float deltaTime, int screenW, int screenH);
    
    static void SetConfig(const AimbotConfig& cfg);
    static AimbotConfig& GetConfig();
    
    static bool IsActive();
    static int GetLockedTarget();
    
private:
    static void ProcessTarget(float deltaTime, int screenW, int screenH);
    static Vec3 GetBonePosition(const EntityData& ent, int hitbox);
    static bool ShouldAim();
    
    static AimbotConfig s_config;
    static bool s_initialized;
    static int s_lockedTarget;
    static float s_remainderX;
    static float s_remainderY;
};

} // namespace Features
