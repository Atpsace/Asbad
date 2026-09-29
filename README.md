# CS2 ESP - External Overlay

**Build 14185** | Optimized external ESP for Counter-Strike 2

## Features

### Visuals
- Box ESP (2D/3D/Corner styles)
- Skeleton ESP with customizable joints
- Health & Armor bars
- Player names & distance
- Weapon ESP
- Head dot indicators
- Eye ray visualization
- Sound ESP
- Offscreen indicators

### World
- Bomb ESP (planted C4, carrier detection)
- Grenade trajectory preview
- Live grenade tracking
- Nade lineups for all maps
- Dropped weapon ESP

### Combat
- Aimbot (FOV-based, smoothing, hitbox selection)
- Triggerbot
- RCS (Recoil Control System)
- Bullet tracers
- Multiple weapon profiles

### Other
- Radar overlay
- Spectator list
- Chams (experimental)
- Multi-monitor support
- Dark/Light theme
- Config save/load system

## Build

### Requirements
- Windows 10/11
- MSYS2 with ucrt64/mingw64 toolchain
- CMake 3.20+ (optional)

### Option 1: PowerShell Build Script
```powershell
.\build.ps1
```

### Option 2: CMake
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Usage

1. Launch CS2
2. Run `esp_fast.exe`
3. Press `INSERT` to toggle menu
4. Configure features in GUI
5. Menu auto-hides when CS2 loses focus

## Configuration

Configs auto-save to `config.cfg` in the executable directory.

**Keybinds:**
- `INSERT` - Toggle menu
- `ALT` - Default aimbot key (configurable)
- `MOUSE4` - Default triggerbot key (configurable)

## Project Structure

```
src/
├── core/           # Process memory, offsets, types
├── features/       # ESP, aimbot, triggerbot modules
├── gui/            # Menu rendering
└── config/         # Config save/load

ImGui LianFlow/     # Custom ImGui fork
base/               # Legacy modules (deprecated)
```

## Offsets

Offsets are hardcoded for **Build 14185**. After CS2 updates:
1. Get new offsets from [sezzyaep/CS2-OFFSETS](https://github.com/sezzyaep/CS2-OFFSETS)
2. Update `src/core/offsets.h`
3. Rebuild

## Notes

- External overlay, no injection
- ReadProcessMemory only (no writes to game)
- VAC detection risk exists - use at own risk
- Designed for learning/research purposes

## Credits

- Offsets: sezzyaep/CS2-OFFSETS
- ImGui: ocornut/imgui
- UI: LianFlow theme (custom)
