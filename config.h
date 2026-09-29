#pragma once
#include <windows.h>
#include <d3d11.h>
#include <string>
#include <algorithm>
#include <cmath>
#include "imgui.h"

enum WeaponCategory {
    WEAPON_GENERAL = 0,
    WEAPON_PISTOL,
    WEAPON_SILENCED_PISTOL,
    WEAPON_RIFLE,
    WEAPON_SNIPER,
    WEAPON_SMG,
    WEAPON_SMG1 = WEAPON_SMG,
    WEAPON_SMG2 = WEAPON_SMG,
    WEAPON_SMG3 = WEAPON_SMG,
    WEAPON_AUTOSNIPER = WEAPON_SNIPER,
    WEAPON_SHOTGUN,
    WEAPON_COUNT
};

static const char* kWeaponProfileNames[] = {
    "General", "Pistol", "Silenced Pistol", "Rifle", "Sniper", "SMG", "Shotgun"
};

struct WeaponConfig {
    bool enabled = true;
    int type = 0;
    int aim_key = 0;
    float fov = 5.0f;
    int hitbox = 0;
    bool smoothing = true;
    float smooth_strength = 4.0f;
    int smooth_type = 0;
    float smooth_humanize = 0.0f;
    int priority = 0;
    bool cond_visible = false;
    bool target_lead = false;
    bool rcs_enabled = false;
    bool rcs_standalone = false;
    int rcs_start_bullet = 1;
    float rcs_yaw = 100.0f;
    float rcs_pitch = 100.0f;
    bool trigger_enabled = false;
    int trigger_key = 0;
    int trigger_min_damage = 1;
    int min_damage = 1;
    float hit_chance = 100.0f;
    bool auto_stop = false;
    int burst_count = 1;
};

struct AppConfig {
    bool is_dark_mode = true;
    float menu_opacity = 0.95f;
    int max_fps = 144;
    int active_weapon = WEAPON_GENERAL;
    bool auto_weapon_detect = true;
    bool driver_active = false;
    char driver_status_text[128] = "Idle";
    bool search_open = false;
    
    WeaponConfig weapons[WEAPON_COUNT];
    
    struct Watermark {
        bool enabled = true;
    } watermark;

    struct Visuals {
        bool enabled = true;
        bool enemies = true;
        bool visible_check = false;
        bool box = true;
        bool box_esp = true;
        int box_style = 0;
        bool box_outline = true;
        bool box_fill = false;
        float box_thickness = 1.5f;
        float box_fill_opacity = 0.2f;
        float box_arm_length = 10.0f;
        float box_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float box_visible_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        float box_occluded_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        bool names = true;
        bool player_names = true;
        float name_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool distance = true;
        bool weapon = true;
        bool weapon_icon = true;
        float weapon_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool health = true;
        bool health_bar = true;
        int health_type = 0;
        bool hp_gradient = false;
        float hp_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        float hp_bar_width = 3.0f;
        int esp_hp_pos = 0;
        bool armor = true;
        bool armor_bar = true;
        bool ammo_bar = true;
        bool flags = false;
        bool snaplines = false;
        float snaplines_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool eye_ray = false;
        float eye_ray_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool head_pos = true;
        float head_pos_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool dropped_weapons = false;
        bool bomb_esp = true;
        bool grenades_esp = true;
        bool grenade_trajectory = true;
        bool sound_esp = true;
        bool offscreen_esp = true;
        bool nade_lineups = true;
        bool bullet_tracers = false;
        bool recoil_crosshair = false;
        bool sniper_crosshair = false;
        bool chams_enabled = false;
        int chams_type = 0;
        float chams_opacity = 0.8f;
        float chams_visible_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        float chams_occluded_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        bool bomb_carrier = false;
        float bomb_carrier_color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
        bool hitmarker = false;
        bool hitsounds = false;
        int vis_check_mode = 0;
        int vis_hysteresis_ms = 0;
    } visuals;

    struct Aimbot {
        bool enabled = false;
        int target_mode = 0;
        int target_bone = 0;
        float fov = 5.0f;
        float smooth = 4.0f;
        bool draw_fov = true;
        float fov_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        bool vis_check = false;
        bool auto_fire = false;
    } aimbot;

    struct Triggerbot {
        bool enabled = false;
        int delay_ms = 40;
    } triggerbot;

    struct Skeleton {
        bool enabled = true;
        int mode = 1; // 1 = Full Skeleton with all joints
        float thickness = 1.5f;
        float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float occluded_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        bool head_circle = true;
        bool joints = true;
    } skeleton;

    struct Spectators {
        bool enabled = true;
    } spectators;

    struct OtherAssists {
        bool auto_pistol = false;
        bool fov_circle = true;
        bool quick_stop = false;
    } other_assists;

    struct Island {
        bool enabled = false;
        bool bomb_active = false;
        float bomb_timer = 0.0f;
        int bomb_site = 0;
        bool defusing = false;
        float defuse_timer = 0.0f;
    } island;
};

extern AppConfig g_Config;

#include <fstream>
#include <sstream>
#include <map>

inline bool SaveConfig(const char* filename = "config.cfg") {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "is_dark_mode=" << (g_Config.is_dark_mode ? 1 : 0) << "\n";
    file << "menu_opacity=" << g_Config.menu_opacity << "\n";
    file << "max_fps=" << g_Config.max_fps << "\n";
    file << "active_weapon=" << g_Config.active_weapon << "\n";
    file << "auto_weapon_detect=" << (g_Config.auto_weapon_detect ? 1 : 0) << "\n";

    // Visuals
    file << "vis_enabled=" << (g_Config.visuals.enabled ? 1 : 0) << "\n";
    file << "vis_enemies=" << (g_Config.visuals.enemies ? 1 : 0) << "\n";
    file << "vis_box=" << (g_Config.visuals.box ? 1 : 0) << "\n";
    file << "vis_box_outline=" << (g_Config.visuals.box_outline ? 1 : 0) << "\n";
    file << "vis_box_fill=" << (g_Config.visuals.box_fill ? 1 : 0) << "\n";
    file << "vis_box_thickness=" << g_Config.visuals.box_thickness << "\n";
    file << "vis_health=" << (g_Config.visuals.health ? 1 : 0) << "\n";
    file << "vis_esp_hp_pos=" << g_Config.visuals.esp_hp_pos << "\n";
    file << "vis_armor=" << (g_Config.visuals.armor ? 1 : 0) << "\n";
    file << "vis_ammo_bar=" << (g_Config.visuals.ammo_bar ? 1 : 0) << "\n";
    file << "vis_names=" << (g_Config.visuals.names ? 1 : 0) << "\n";
    file << "vis_distance=" << (g_Config.visuals.distance ? 1 : 0) << "\n";
    file << "vis_weapon=" << (g_Config.visuals.weapon ? 1 : 0) << "\n";
    file << "vis_flags=" << (g_Config.visuals.flags ? 1 : 0) << "\n";
    file << "vis_offscreen_esp=" << (g_Config.visuals.offscreen_esp ? 1 : 0) << "\n";
    file << "vis_sound_esp=" << (g_Config.visuals.sound_esp ? 1 : 0) << "\n";
    file << "vis_head_pos=" << (g_Config.visuals.head_pos ? 1 : 0) << "\n";
    file << "vis_eye_ray=" << (g_Config.visuals.eye_ray ? 1 : 0) << "\n";
    file << "vis_snaplines=" << (g_Config.visuals.snaplines ? 1 : 0) << "\n";
    file << "vis_grenades_esp=" << (g_Config.visuals.grenades_esp ? 1 : 0) << "\n";
    file << "vis_grenade_trajectory=" << (g_Config.visuals.grenade_trajectory ? 1 : 0) << "\n";
    file << "vis_nade_lineups=" << (g_Config.visuals.nade_lineups ? 1 : 0) << "\n";
    file << "vis_bomb_esp=" << (g_Config.visuals.bomb_esp ? 1 : 0) << "\n";
    file << "vis_bomb_carrier=" << (g_Config.visuals.bomb_carrier ? 1 : 0) << "\n";
    file << "vis_dropped_weapons=" << (g_Config.visuals.dropped_weapons ? 1 : 0) << "\n";
    file << "vis_bullet_tracers=" << (g_Config.visuals.bullet_tracers ? 1 : 0) << "\n";
    file << "vis_recoil_crosshair=" << (g_Config.visuals.recoil_crosshair ? 1 : 0) << "\n";
    file << "vis_sniper_crosshair=" << (g_Config.visuals.sniper_crosshair ? 1 : 0) << "\n";

    // Colors
    file << "col_box_vis=" << g_Config.visuals.box_visible_color[0] << "," << g_Config.visuals.box_visible_color[1] << "," << g_Config.visuals.box_visible_color[2] << "," << g_Config.visuals.box_visible_color[3] << "\n";
    file << "col_box_occ=" << g_Config.visuals.box_color[0] << "," << g_Config.visuals.box_color[1] << "," << g_Config.visuals.box_color[2] << "," << g_Config.visuals.box_color[3] << "\n";
    file << "col_skel=" << g_Config.skeleton.color[0] << "," << g_Config.skeleton.color[1] << "," << g_Config.skeleton.color[2] << "," << g_Config.skeleton.color[3] << "\n";
    file << "col_head=" << g_Config.visuals.head_pos_color[0] << "," << g_Config.visuals.head_pos_color[1] << "," << g_Config.visuals.head_pos_color[2] << "," << g_Config.visuals.head_pos_color[3] << "\n";
    file << "col_snap=" << g_Config.visuals.snaplines_color[0] << "," << g_Config.visuals.snaplines_color[1] << "," << g_Config.visuals.snaplines_color[2] << "," << g_Config.visuals.snaplines_color[3] << "\n";

    // Skeleton & Spectator
    file << "skel_enabled=" << (g_Config.skeleton.enabled ? 1 : 0) << "\n";
    file << "skel_joints=" << (g_Config.skeleton.joints ? 1 : 0) << "\n";
    file << "skel_thickness=" << g_Config.skeleton.thickness << "\n";
    file << "spec_enabled=" << (g_Config.spectators.enabled ? 1 : 0) << "\n";

    // Aimbot & Triggerbot
    file << "aim_enabled=" << (g_Config.aimbot.enabled ? 1 : 0) << "\n";
    file << "aim_fov=" << g_Config.aimbot.fov << "\n";
    file << "aim_smooth=" << g_Config.aimbot.smooth << "\n";
    file << "aim_draw_fov=" << (g_Config.aimbot.draw_fov ? 1 : 0) << "\n";
    file << "aim_vis_check=" << (g_Config.aimbot.vis_check ? 1 : 0) << "\n";
    file << "aim_auto_fire=" << (g_Config.aimbot.auto_fire ? 1 : 0) << "\n";
    file << "trig_enabled=" << (g_Config.triggerbot.enabled ? 1 : 0) << "\n";
    file << "trig_delay=" << g_Config.triggerbot.delay_ms << "\n";
    file << "auto_pistol=" << (g_Config.other_assists.auto_pistol ? 1 : 0) << "\n";
    file << "island_enabled=" << (g_Config.island.enabled ? 1 : 0) << "\n";

    file.close();
    return true;
}

inline bool LoadConfig(const char* filename = "config.cfg") {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::map<std::string, std::string> kv;
    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            kv[line.substr(0, eq)] = line.substr(eq + 1);
        }
    }
    file.close();

    auto parseBool = [&](const char* k, bool defVal) -> bool {
        auto it = kv.find(k);
        if (it == kv.end()) return defVal;
        return std::stoi(it->second) != 0;
    };
    auto parseFloat = [&](const char* k, float defVal) -> float {
        auto it = kv.find(k);
        if (it == kv.end()) return defVal;
        return std::stof(it->second);
    };
    auto parseInt = [&](const char* k, int defVal) -> int {
        auto it = kv.find(k);
        if (it == kv.end()) return defVal;
        return std::stoi(it->second);
    };
    auto parseColor = [&](const char* k, float col[4]) {
        auto it = kv.find(k);
        if (it == kv.end()) return;
        std::stringstream ss(it->second);
        std::string token;
        int idx = 0;
        while (std::getline(ss, token, ',') && idx < 4) {
            col[idx++] = std::stof(token);
        }
    };

    g_Config.is_dark_mode = parseBool("is_dark_mode", g_Config.is_dark_mode);
    g_Config.menu_opacity = parseFloat("menu_opacity", g_Config.menu_opacity);
    g_Config.max_fps = parseInt("max_fps", g_Config.max_fps);
    g_Config.active_weapon = parseInt("active_weapon", g_Config.active_weapon);
    g_Config.auto_weapon_detect = parseBool("auto_weapon_detect", g_Config.auto_weapon_detect);

    g_Config.visuals.enabled = parseBool("vis_enabled", g_Config.visuals.enabled);
    g_Config.visuals.enemies = parseBool("vis_enemies", g_Config.visuals.enemies);
    g_Config.visuals.box = parseBool("vis_box", g_Config.visuals.box);
    g_Config.visuals.box_esp = g_Config.visuals.box;
    g_Config.visuals.box_outline = parseBool("vis_box_outline", g_Config.visuals.box_outline);
    g_Config.visuals.box_fill = parseBool("vis_box_fill", g_Config.visuals.box_fill);
    g_Config.visuals.box_thickness = parseFloat("vis_box_thickness", g_Config.visuals.box_thickness);
    g_Config.visuals.health = parseBool("vis_health", g_Config.visuals.health);
    g_Config.visuals.health_bar = g_Config.visuals.health;
    g_Config.visuals.esp_hp_pos = parseInt("vis_esp_hp_pos", g_Config.visuals.esp_hp_pos);
    g_Config.visuals.armor = parseBool("vis_armor", g_Config.visuals.armor);
    g_Config.visuals.armor_bar = g_Config.visuals.armor;
    g_Config.visuals.ammo_bar = parseBool("vis_ammo_bar", g_Config.visuals.ammo_bar);
    g_Config.visuals.names = parseBool("vis_names", g_Config.visuals.names);
    g_Config.visuals.player_names = g_Config.visuals.names;
    g_Config.visuals.distance = parseBool("vis_distance", g_Config.visuals.distance);
    g_Config.visuals.weapon = parseBool("vis_weapon", g_Config.visuals.weapon);
    g_Config.visuals.weapon_icon = g_Config.visuals.weapon;
    g_Config.visuals.flags = parseBool("vis_flags", g_Config.visuals.flags);
    g_Config.visuals.offscreen_esp = parseBool("vis_offscreen_esp", g_Config.visuals.offscreen_esp);
    g_Config.visuals.sound_esp = parseBool("vis_sound_esp", g_Config.visuals.sound_esp);
    g_Config.visuals.head_pos = parseBool("vis_head_pos", g_Config.visuals.head_pos);
    g_Config.visuals.eye_ray = parseBool("vis_eye_ray", g_Config.visuals.eye_ray);
    g_Config.visuals.snaplines = parseBool("vis_snaplines", g_Config.visuals.snaplines);
    g_Config.visuals.grenades_esp = parseBool("vis_grenades_esp", g_Config.visuals.grenades_esp);
    g_Config.visuals.grenade_trajectory = parseBool("vis_grenade_trajectory", g_Config.visuals.grenade_trajectory);
    g_Config.visuals.nade_lineups = parseBool("vis_nade_lineups", g_Config.visuals.nade_lineups);
    g_Config.visuals.bomb_esp = parseBool("vis_bomb_esp", g_Config.visuals.bomb_esp);
    g_Config.visuals.bomb_carrier = parseBool("vis_bomb_carrier", g_Config.visuals.bomb_carrier);
    g_Config.visuals.dropped_weapons = parseBool("vis_dropped_weapons", g_Config.visuals.dropped_weapons);
    g_Config.visuals.bullet_tracers = parseBool("vis_bullet_tracers", g_Config.visuals.bullet_tracers);
    g_Config.visuals.recoil_crosshair = parseBool("vis_recoil_crosshair", g_Config.visuals.recoil_crosshair);
    g_Config.visuals.sniper_crosshair = parseBool("vis_sniper_crosshair", g_Config.visuals.sniper_crosshair);

    parseColor("col_box_vis", g_Config.visuals.box_visible_color);
    parseColor("col_box_occ", g_Config.visuals.box_color);
    parseColor("col_skel", g_Config.skeleton.color);
    parseColor("col_head", g_Config.visuals.head_pos_color);
    parseColor("col_snap", g_Config.visuals.snaplines_color);

    g_Config.skeleton.enabled = parseBool("skel_enabled", g_Config.skeleton.enabled);
    g_Config.skeleton.joints = parseBool("skel_joints", g_Config.skeleton.joints);
    g_Config.skeleton.thickness = parseFloat("skel_thickness", g_Config.skeleton.thickness);
    g_Config.spectators.enabled = parseBool("spec_enabled", g_Config.spectators.enabled);

    g_Config.aimbot.enabled = parseBool("aim_enabled", g_Config.aimbot.enabled);
    g_Config.aimbot.fov = parseFloat("aim_fov", g_Config.aimbot.fov);
    g_Config.aimbot.smooth = parseFloat("aim_smooth", g_Config.aimbot.smooth);
    g_Config.aimbot.draw_fov = parseBool("aim_draw_fov", g_Config.aimbot.draw_fov);
    g_Config.aimbot.vis_check = parseBool("aim_vis_check", g_Config.aimbot.vis_check);
    g_Config.aimbot.auto_fire = parseBool("aim_auto_fire", g_Config.aimbot.auto_fire);
    g_Config.triggerbot.enabled = parseBool("trig_enabled", g_Config.triggerbot.enabled);
    g_Config.triggerbot.delay_ms = parseInt("trig_delay", g_Config.triggerbot.delay_ms);
    g_Config.other_assists.auto_pistol = parseBool("auto_pistol", g_Config.other_assists.auto_pistol);
    g_Config.island.enabled = parseBool("island_enabled", g_Config.island.enabled);

    return true;
}

namespace HudGui {
    inline void RenderOverlays(int specCount, const char specs[][64], bool menuOpen, float speed, bool inAir) {}
}

namespace DynamicIsland {
    inline void Render() {}
}

namespace stealth {
    inline const wchar_t* spoof_class_name() { return L"Direct3DWindowClass"; }
    inline const wchar_t* spoof_window_title() { return L"CS2 Overlay"; }
    inline void init(HANDLE hInput, HWND hwnd) {}
}

namespace HelloIntro {
    inline void Init(ID3D11Device* dev) {}
}

namespace ReactiveState {
    inline void InitializeDefaultSubscribers() {}
    inline void DispatchToggle(int id, bool val) {}
}

#define FEATURE_INTERFACE_DARK_MODE 1

static unsigned char Inter_SemmiBold[1] = {0};
static unsigned char Icon_Pack[1] = {0};
