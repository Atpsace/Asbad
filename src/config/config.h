#pragma once
#include "../core/types.h"
#include <string>

namespace Config {

struct WeaponProfile {
    bool enabled = true;
    int aimKey = 0x12; // VK_MENU (ALT)
    float fov = 5.0f;
    int hitbox = 0; // Head
    float smoothing = 4.0f;
    bool visCheck = false;
    bool rcsEnabled = false;
    float rcsYaw = 100.0f;
    float rcsPitch = 100.0f;
};

struct Settings {
    // Visuals
    bool espEnabled = true;
    bool boxes = true;
    int boxStyle = 0; // 0=2D, 1=3D, 2=Corner
    bool skeleton = true;
    bool health = true;
    bool armor = true;
    bool names = true;
    bool weapon = true;
    bool distance = true;
    bool headDot = true;
    bool eyeRay = false;
    bool snaplines = false;
    bool soundEsp = true;
    bool offscreenEsp = true;
    bool teamCheck = true;
    
    // World
    bool bombEsp = true;
    bool nadeTrajectory = true;
    bool liveNades = true;
    bool nadeLineups = true;
    bool worldLoot = false;
    
    // Combat
    bool aimbotEnabled = false;
    bool triggerbotEnabled = false;
    WeaponProfile weaponGeneral;
    
    // Other
    bool showSpectators = true;
    bool darkMode = true;
    float menuOpacity = 0.95f;
};

bool Load(const std::string& path, Settings& settings);
bool Save(const std::string& path, const Settings& settings);
std::string GetDefaultConfigPath();

} // namespace Config
