#include "config.h"
#include <fstream>
#include <sstream>
#include <windows.h>

namespace Config {

std::string GetDefaultConfigPath() {
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string path(exePath);
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
        return path.substr(0, pos + 1) + "config.cfg";
    return "config.cfg";
}

bool Load(const std::string& path, Settings& settings) {
    std::ifstream file(path);
    if (!file.is_open()) return false;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);
        
        if (key == "esp") settings.espEnabled = (val == "1");
        else if (key == "boxes") settings.boxes = (val == "1");
        else if (key == "box_style") settings.boxStyle = std::stoi(val);
        else if (key == "skeleton") settings.skeleton = (val == "1");
        else if (key == "health") settings.health = (val == "1");
        else if (key == "armor") settings.armor = (val == "1");
        else if (key == "names") settings.names = (val == "1");
        else if (key == "weapon") settings.weapon = (val == "1");
        else if (key == "distance") settings.distance = (val == "1");
        else if (key == "head_dot") settings.headDot = (val == "1");
        else if (key == "eye_ray") settings.eyeRay = (val == "1");
        else if (key == "snaplines") settings.snaplines = (val == "1");
        else if (key == "sound_esp") settings.soundEsp = (val == "1");
        else if (key == "offscreen") settings.offscreenEsp = (val == "1");
        else if (key == "team_check") settings.teamCheck = (val == "1");
        else if (key == "bomb") settings.bombEsp = (val == "1");
        else if (key == "nade_traj") settings.nadeTrajectory = (val == "1");
        else if (key == "live_nades") settings.liveNades = (val == "1");
        else if (key == "lineups") settings.nadeLineups = (val == "1");
        else if (key == "loot") settings.worldLoot = (val == "1");
        else if (key == "aimbot") settings.aimbotEnabled = (val == "1");
        else if (key == "trigger") settings.triggerbotEnabled = (val == "1");
        else if (key == "aim_fov") settings.weaponGeneral.fov = std::stof(val);
        else if (key == "aim_smooth") settings.weaponGeneral.smoothing = std::stof(val);
        else if (key == "spectators") settings.showSpectators = (val == "1");
        else if (key == "dark_mode") settings.darkMode = (val == "1");
        else if (key == "opacity") settings.menuOpacity = std::stof(val);
    }
    
    return true;
}

bool Save(const std::string& path, const Settings& settings) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    
    file << "# CS2 ESP Config\n";
    file << "esp=" << (settings.espEnabled ? 1 : 0) << "\n";
    file << "boxes=" << (settings.boxes ? 1 : 0) << "\n";
    file << "box_style=" << settings.boxStyle << "\n";
    file << "skeleton=" << (settings.skeleton ? 1 : 0) << "\n";
    file << "health=" << (settings.health ? 1 : 0) << "\n";
    file << "armor=" << (settings.armor ? 1 : 0) << "\n";
    file << "names=" << (settings.names ? 1 : 0) << "\n";
    file << "weapon=" << (settings.weapon ? 1 : 0) << "\n";
    file << "distance=" << (settings.distance ? 1 : 0) << "\n";
    file << "head_dot=" << (settings.headDot ? 1 : 0) << "\n";
    file << "eye_ray=" << (settings.eyeRay ? 1 : 0) << "\n";
    file << "snaplines=" << (settings.snaplines ? 1 : 0) << "\n";
    file << "sound_esp=" << (settings.soundEsp ? 1 : 0) << "\n";
    file << "offscreen=" << (settings.offscreenEsp ? 1 : 0) << "\n";
    file << "team_check=" << (settings.teamCheck ? 1 : 0) << "\n";
    file << "bomb=" << (settings.bombEsp ? 1 : 0) << "\n";
    file << "nade_traj=" << (settings.nadeTrajectory ? 1 : 0) << "\n";
    file << "live_nades=" << (settings.liveNades ? 1 : 0) << "\n";
    file << "lineups=" << (settings.nadeLineups ? 1 : 0) << "\n";
    file << "loot=" << (settings.worldLoot ? 1 : 0) << "\n";
    file << "aimbot=" << (settings.aimbotEnabled ? 1 : 0) << "\n";
    file << "trigger=" << (settings.triggerbotEnabled ? 1 : 0) << "\n";
    file << "aim_fov=" << settings.weaponGeneral.fov << "\n";
    file << "aim_smooth=" << settings.weaponGeneral.smoothing << "\n";
    file << "spectators=" << (settings.showSpectators ? 1 : 0) << "\n";
    file << "dark_mode=" << (settings.darkMode ? 1 : 0) << "\n";
    file << "opacity=" << settings.menuOpacity << "\n";
    
    return true;
}

} // namespace Config
