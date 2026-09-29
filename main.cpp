// main.cpp — CS2 ESP (OPTIMIZED BUILD)
// Build 14178 verified offsets | Extended: aimbot, triggerbot, bomb, loot,
// sound, spectators
#include <d3d11.h>
#include <dwmapi.h>
#include <immintrin.h>
#include <intrin.h>
#include <timeapi.h>
#include <tlhelp32.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Font.h"
#include "config.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "matchio.h"
#include "obf.h"
#include "theme.h"
#include "ImGui LianFlow/examples/example_win32_directx11/main.h"
#include "ImGui LianFlow/examples/example_win32_directx11/directx_blur.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM,
                                                             LPARAM);

// RAII for Win32 HANDLE so snapshots/process opens cannot leak on early return.
struct UniqueHandle {
  HANDLE h = NULL;
  UniqueHandle() = default;
  explicit UniqueHandle(HANDLE handle) : h(handle) {}
  ~UniqueHandle() { reset(); }
  UniqueHandle(const UniqueHandle &) = delete;
  UniqueHandle &operator=(const UniqueHandle &) = delete;
  UniqueHandle(UniqueHandle &&o) noexcept : h(o.h) { o.h = NULL; }
  UniqueHandle &operator=(UniqueHandle &&o) noexcept {
    if (this != &o) {
      reset();
      h = o.h;
      o.h = NULL;
    }
    return *this;
  }
  void reset(HANDLE nh = NULL) {
    if (h && h != INVALID_HANDLE_VALUE)
      CloseHandle(h);
    h = nh;
  }
  HANDLE get() const { return h; }
  explicit operator bool() const {
    return h && h != INVALID_HANDLE_VALUE;
  }
};

// ============ CS2 OFFSETS (Build 14185 from sezzyaep/CS2-OFFSETS) ============
namespace O {
constexpr uintptr_t dwEntityList = 0x27151E8;
constexpr uintptr_t dwLocalPlayerPawn = 0x25606D8;
constexpr uintptr_t dwLocalPlayerController = 0x2537628;
constexpr uintptr_t dwViewMatrix = 0x2565A20;
constexpr uintptr_t dwPlantedC4 = 0x24C9290;
constexpr uintptr_t dwWeaponC4 = 0x24C4550;
constexpr uintptr_t dwGlobalVars = 0x222BF88;
constexpr uintptr_t dwViewAngles = 0x2576238;
constexpr uintptr_t dwSensitivity = 0x255D428;
constexpr uintptr_t dwSensitivity_sensitivity = 0x58;

constexpr uintptr_t m_hPlayerPawn = 0x92C;
constexpr uintptr_t m_bPawnIsAlive = 0x934;
constexpr uintptr_t m_iszPlayerName = 0x6FC;
constexpr uintptr_t m_iHealth = 0x34C;
constexpr uintptr_t m_iTeamNum = 0x3E7;
constexpr uintptr_t m_pGameSceneNode = 0x330;
constexpr uintptr_t m_modelState = 0x140;
constexpr uintptr_t m_boneArray = 0x1C0;
constexpr uintptr_t m_vecAbsOrigin = 0xC8;
constexpr uintptr_t m_vOldOrigin = 0x14A4;

constexpr uintptr_t m_hOwnerEntity = 0x520;
constexpr uintptr_t m_pWeaponSvc = 0x12F0;
constexpr uintptr_t m_hActiveWpn = 0x60;
constexpr uintptr_t m_AttrMgr = 0x1290;
constexpr uintptr_t m_Item_off = 0x50;
constexpr uintptr_t m_iItemDefIdx = 0x1BA;
constexpr uintptr_t m_vecViewOffset = 0xF60;
constexpr uintptr_t m_bPinPulled = 0x1F23;
constexpr uintptr_t m_flThrowStrength = 0x1F30;
constexpr uintptr_t m_vInitialPosition = 0x12B0;
constexpr uintptr_t m_vInitialVelocity = 0x12BC;
constexpr uintptr_t m_nBounces = 0x12C8;
constexpr uintptr_t m_flSpawnTime = 0x12E8;
constexpr uintptr_t m_bExplodeEffectBegan = 0x12FC;
constexpr uintptr_t m_arrTrajectoryPoints = 0x1310;
constexpr uintptr_t m_pClippingWeapon = 0x13A0;
constexpr uintptr_t m_pEntity_id = 0x10;
constexpr uintptr_t m_designerName = 0x20;
constexpr uintptr_t m_pObserverSvc = 0x1308;
constexpr uintptr_t m_hObsTgt = 0x4C;
constexpr uintptr_t m_entitySpottedState = 0x1E88;
constexpr uintptr_t m_bSpotted = 0x8;
constexpr uintptr_t m_bSpottedByMask = 0xC;
constexpr uintptr_t m_emitSndTime = 0x1EA8;
constexpr uintptr_t m_absVelocity = 0x3F8;
constexpr uintptr_t m_fFlags = 0x3F4;
constexpr uintptr_t m_pAimPunchServices = 0x1598;
constexpr uintptr_t m_predictableBaseAngle = 0x50;
constexpr uintptr_t m_pCameraServices = 0x1328;
constexpr uintptr_t m_vecCsViewPunchAngle = 0x48;
constexpr uintptr_t m_aimPunchAngleDirect = 0x15B0;
constexpr uintptr_t m_iShotsFired = 0x1EB4;
constexpr uintptr_t m_angEyeAngles = 0x35F0;
constexpr uintptr_t m_ArmorValue = 0x1ECC;
constexpr uintptr_t m_iClip1 = 0x1928;

constexpr uintptr_t c4_bTicking = 0x1288;
constexpr uintptr_t c4_nSite = 0x128C;
constexpr uintptr_t c4_flBlow = 0x12B8;
constexpr uintptr_t c4_flTimer = 0x12C0;
constexpr uintptr_t c4_bDefusing = 0x12C4;
constexpr uintptr_t c4_flDefuseLen = 0x12D4;
constexpr uintptr_t c4_flDefuseCountDown = 0x12D8;
constexpr uintptr_t c4_bDefused = 0x12DC;
} // namespace O

// ============ TIMER GUARD ============
struct TimerGuard {
  bool ok = false;
  TimerGuard() { ok = (timeBeginPeriod(1) == TIMERR_NOERROR); }
  ~TimerGuard() {
    if (ok)
      timeEndPeriod(1);
  }
};

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

struct HighPrecisionSleep {
  HANDLE hTimer = NULL;
  HighPrecisionSleep() {
    hTimer = CreateWaitableTimerExW(
        NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
    if (!hTimer)
      hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
  }
  ~HighPrecisionSleep() {
    if (hTimer) {
      CloseHandle(hTimer);
      hTimer = NULL;
    }
  }
  void SleepMicros(long long micros) {
    if (micros <= 0)
      return;
    if (hTimer) {
      LARGE_INTEGER dueTime;
      dueTime.QuadPart = -10LL * micros;
      if (SetWaitableTimer(hTimer, &dueTime, 0, NULL, NULL, 0)) {
        const DWORD waitMs = (DWORD)((micros + 999) / 1000);
        WaitForSingleObject(hTimer, waitMs + 2);
        return;
      }
    }
    if (micros >= 1000)
      Sleep((DWORD)(micros / 1000));
  }
};

// ============ CONSOLE LOGGER ============
namespace ConsoleLogger {
static HANDLE s_hConsole = NULL;
static CRITICAL_SECTION s_logLock;
static bool s_initialized = false;

inline void Init() {
  if (s_initialized)
    return;
  InitializeCriticalSection(&s_logLock);
  s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  s_initialized = true;
}
enum LogLevel {
  LOG_INFO,
  LOG_SUCCESS,
  LOG_WARN,
  LOG_ERROR,
  LOG_OFFSET,
  LOG_COMBAT
};

inline void Log(LogLevel level, const char *fmt, ...) {
  if (!s_hConsole)
    s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!s_hConsole || s_hConsole == INVALID_HANDLE_VALUE)
    return;
  if (!s_initialized)
    Init();
  EnterCriticalSection(&s_logLock);
  SYSTEMTIME st;
  GetLocalTime(&st);
  SetConsoleTextAttribute(s_hConsole, FOREGROUND_INTENSITY);
  printf("[%02d:%02d:%02d] ", st.wHour, st.wMinute, st.wSecond);
  switch (level) {
  case LOG_INFO:
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |
                                            FOREGROUND_INTENSITY);
    printf("[*] ");
    break;
  case LOG_SUCCESS:
    SetConsoleTextAttribute(s_hConsole,
                            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("[+] ");
    break;
  case LOG_WARN:
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                            FOREGROUND_INTENSITY);
    printf("[!] ");
    break;
  case LOG_ERROR:
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("[-] ");
    break;
  case LOG_OFFSET:
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_INTENSITY);
    printf("[OFFSET] ");
    break;
  case LOG_COMBAT:
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                            FOREGROUND_INTENSITY);
    printf("[COMBAT] ");
    break;
  }
  SetConsoleTextAttribute(s_hConsole,
                          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
  LeaveCriticalSection(&s_logLock);
}
inline void PrintOffsetTable(uintptr_t base) {
  if (!s_hConsole)
    s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!s_hConsole || s_hConsole == INVALID_HANDLE_VALUE)
    return;
  if (!s_initialized)
    Init();
  EnterCriticalSection(&s_logLock);
  SetConsoleTextAttribute(s_hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE |
                                          FOREGROUND_INTENSITY);
  printf("\n  "
         "+--------------------------------------------------------------------"
         "--+\n");
  printf(
      "  |  ACTIVE CS2 CLIENT OFFSETS (client.dll: 0x%012llX)             |\n",
      (unsigned long long)base);
  printf("  "
         "+--------------------------------------------------------------------"
         "--+\n");
  auto printRow = [](const char *name, uintptr_t offset, uintptr_t absAddr) {
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_INTENSITY);
    printf("  |  %-24s ", name);
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_INTENSITY);
    printf("[+0x%07X] -> ", (unsigned int)offset);
    SetConsoleTextAttribute(s_hConsole,
                            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("0x%012llX", (unsigned long long)absAddr);
    SetConsoleTextAttribute(s_hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE |
                                            FOREGROUND_INTENSITY);
    printf("  |\n");
  };
  printRow("dwEntityList", O::dwEntityList, base + O::dwEntityList);
  printRow("dwLocalPlayerPawn", O::dwLocalPlayerPawn,
           base + O::dwLocalPlayerPawn);
  printRow("dwLocalPlayerController", O::dwLocalPlayerController,
           base + O::dwLocalPlayerController);
  printRow("dwViewMatrix", O::dwViewMatrix, base + O::dwViewMatrix);
  printRow("dwViewAngles", O::dwViewAngles, base + O::dwViewAngles);
  printRow("dwSensitivity", O::dwSensitivity, base + O::dwSensitivity);
  printRow("dwPlantedC4", O::dwPlantedC4, base + O::dwPlantedC4);
  printRow("dwWeaponC4", O::dwWeaponC4, base + O::dwWeaponC4);
  printRow("dwGlobalVars", O::dwGlobalVars, base + O::dwGlobalVars);
  SetConsoleTextAttribute(s_hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE |
                                          FOREGROUND_INTENSITY);
  printf("  "
         "+--------------------------------------------------------------------"
         "--+\n\n");
  SetConsoleTextAttribute(s_hConsole,
                          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
  LeaveCriticalSection(&s_logLock);
}
} // namespace ConsoleLogger

// ============ PROCESS GLOBALS ============
HANDLE hGame = NULL;
DWORD pid = 0;
uintptr_t clientBase = 0;
float g_gameSens = 1.25f;
HWND g_hwndCS2 = NULL;

std::atomic<bool> g_config_dirty{false};
constexpr float kRcsPunchScale = 2.0f;

// ============ FEATURE FLAGS ============
bool menuOpen = false;
bool g_masterEnabled = true;
bool espEnabled = true;
bool drawBoxes = true;
int boxStyle = 0;
bool drawSkeletons = true;
int skeletonStyle = 1;
bool drawHeadDot = true;
bool drawHealth = true;
bool drawArmor = true;
bool drawNames = true;
bool drawDistance = true;
bool drawSnaplines = false;
bool drawWeapon = true;
bool drawEyeRay = false;
bool teamCheck = true;
bool drawFlags = false;
int colorPalette = 0;
bool soundEsp = true;
bool bombEsp = true;
bool droppedC4Esp = true;
bool nadeLineups = true;
int lineupShowAllMode = 0;
bool liveNades = true;
bool nadeTrajectory = true;
bool offscreenEsp = true;
bool drawAmmoBar = true;
int selectedMapIdx = 0;
bool showSpectators = true;
bool worldLoot = false;

// ============ AIMBOT CONFIG ============
bool aimbotEnabled = false;
int aimbotKey = 0;
int g_aimKeyVk = VK_LMENU;
int g_triggerKeyVk = VK_XBUTTON1;
float aimbotFovDeg = 5.0f;
float aimbotSmooth = 4.0f;
int aimbotBone = 0;
bool aimbotFovCircle = true;
bool aimbotLead = false;
bool aimbotWallCheck = false;
bool bulletTracer = false;
float bulletColor[4] = {0.0f, 0.584f, 1.0f, 1.0f};
int radarDistance = 60;
bool chamsEnabled = false;
int chamsType = 0;
float chamsColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
int chamsDistance = 60;

int aimbotTargetPrio = 0;
bool autoStopEnabled = false;

// ============ TRIGGERBOT CONFIG ============
bool triggerbotEnabled = false;
int triggerMode = 0;
int triggerDelay = 40;
float triggerRadius = 22.0f;

// ============ CONFIG SYSTEM ============
bool autoSaveConfig = true;
bool autoLoadConfig = true;
static char g_lastConfigStatus[128] = "Idle";
static int g_selectedPresetIdx = 0;

static std::string GetConfigFilePath(const char *filename = "config.cfg") {
  char exePath[MAX_PATH] = {0};
  GetModuleFileNameA(NULL, exePath, MAX_PATH);
  std::string path(exePath);
  size_t pos = path.find_last_of("\\/");
  if (pos != std::string::npos)
    return path.substr(0, pos + 1) + filename;
  return std::string(filename);
}

static void ResetConfigDefaults() {
  g_masterEnabled = true;
  colorPalette = 0;
  autoSaveConfig = true;
  autoLoadConfig = true;
  espEnabled = true;
  drawBoxes = true;
  boxStyle = 0;
  drawSkeletons = true;
  skeletonStyle = 1;
  g_Config.skeleton.mode = 1;
  g_Config.skeleton.joints = true;
  drawHeadDot = true;
  drawHealth = true;
  drawArmor = true;
  drawNames = true;
  drawDistance = true;
  drawSnaplines = false;
  drawWeapon = true;
  drawEyeRay = false;
  teamCheck = true;
  soundEsp = true;
  bombEsp = true;
  droppedC4Esp = true;
  nadeLineups = true;
  liveNades = true;
  nadeTrajectory = true;
  offscreenEsp = true;
  drawAmmoBar = true;
  selectedMapIdx = 0;
  showSpectators = true;
  worldLoot = false;
  aimbotEnabled = false;
  aimbotKey = 0;
  aimbotFovDeg = 5.0f;
  aimbotSmooth = 4.0f;
  aimbotBone = 0;
  aimbotFovCircle = true;
  aimbotLead = false;
  aimbotWallCheck = false;
  bulletTracer = false;
  bulletColor[0] = 0.0f;
  bulletColor[1] = 0.584f;
  bulletColor[2] = 1.0f;
  bulletColor[3] = 1.0f;
  radarDistance = 60;
  chamsEnabled = false;
  chamsType = 0;
  chamsColor[0] = 1.0f;
  chamsColor[1] = 0.0f;
  chamsColor[2] = 0.0f;
  chamsColor[3] = 1.0f;
  chamsDistance = 60;
  triggerbotEnabled = false;
  triggerMode = 0;
  triggerDelay = 40;
  triggerRadius = 22.0f;
  aimbotTargetPrio = 0;
  autoStopEnabled = false;
  snprintf(g_lastConfigStatus, sizeof(g_lastConfigStatus), "Defaults restored");
}

// ============ NEWGUI GLOBALS ============
AppConfig g_Config;
ImFont *g_FontLight = nullptr;
ImFont *g_FontRegular = nullptr;
ImFont *g_FontSemibold = nullptr;
ImFont *g_FontBold = nullptr;
ImFont *g_FontBrandTitle = nullptr;
ImFont *g_FontHeader = nullptr;
ImFont *g_FontSection = nullptr;
ImFont *g_FontNav = nullptr;
ImFont *g_FontNavActive = nullptr;
ImFont *g_FontBrandVer = nullptr;
ImFont *g_FontSubNav = nullptr;
ImFont *g_FontSubNavActive = nullptr;
ImFont *g_FontSmall = nullptr;
ImFont *g_FontSmallBold = nullptr;
ImFont *g_EspFonts[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
const char *g_EspFontNames[6] = {"Segoe UI", "Tahoma",   "Verdana",
                                 "Arial",    "Consolas", "Trebuchet MS"};

void SyncConfigToGlobals() {
  int activeWpnIdx =
      (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT)
          ? g_Config.active_weapon
          : WEAPON_GENERAL;
  WeaponConfig &curWcfg = g_Config.weapons[activeWpnIdx];
  const WeaponConfig &wcfg = (curWcfg.enabled || activeWpnIdx == WEAPON_GENERAL)
                                 ? curWcfg
                                 : (g_Config.weapons[WEAPON_GENERAL].enabled
                                        ? g_Config.weapons[WEAPON_GENERAL]
                                        : curWcfg);
  g_Config.aimbot.enabled = wcfg.enabled || g_Config.weapons[WEAPON_GENERAL].enabled;
  aimbotEnabled = g_Config.aimbot.enabled;
  aimbotFovDeg = (wcfg.fov > 0.1f) ? wcfg.fov : 5.0f;
  aimbotBone = std::clamp(wcfg.hitbox, 0, 3);
  aimbotSmooth =
      wcfg.smoothing
          ? ((wcfg.smooth_strength >= 1.0f) ? wcfg.smooth_strength : 1.0f)
          : 1.0f;
  aimbotTargetPrio = wcfg.priority;
  g_Config.aimbot.vis_check = wcfg.cond_visible;
  aimbotWallCheck = wcfg.cond_visible;
  if (wcfg.type == 0) {
    g_aimKeyVk = VK_LBUTTON;
    aimbotKey = 4;
  } else if (wcfg.type == 2) {
    g_aimKeyVk = -1;
    aimbotKey = 8;
  } else {
    g_aimKeyVk = (wcfg.aim_key > 0) ? wcfg.aim_key : VK_LMENU;
    aimbotKey = 0;
  }
  autoStopEnabled = wcfg.auto_stop;
  g_Config.triggerbot.enabled = wcfg.trigger_enabled;
  triggerbotEnabled = wcfg.trigger_enabled;
  g_triggerKeyVk = (wcfg.trigger_key > 0) ? wcfg.trigger_key : VK_XBUTTON1;
  espEnabled = g_Config.visuals.enabled;
  g_Config.visuals.box_esp = g_Config.visuals.box;
  drawBoxes = g_Config.visuals.box;
  boxStyle = g_Config.visuals.box_style;
  g_Config.visuals.health_bar = g_Config.visuals.health;
  drawHealth = g_Config.visuals.health;
  g_Config.visuals.player_names = g_Config.visuals.names;
  drawNames = g_Config.visuals.names;
  g_Config.visuals.weapon_icon = g_Config.visuals.weapon;
  drawWeapon = g_Config.visuals.weapon;
  g_Config.visuals.armor_bar = g_Config.visuals.armor;
  drawArmor = g_Config.visuals.armor;
  worldLoot = g_Config.visuals.dropped_weapons;
  bombEsp = g_Config.visuals.bomb_esp;
  droppedC4Esp = g_Config.visuals.bomb_carrier || g_Config.visuals.bomb_esp;
  liveNades = g_Config.visuals.grenades_esp;
  nadeTrajectory = g_Config.visuals.grenade_trajectory;
  soundEsp = g_Config.visuals.sound_esp;
  offscreenEsp = g_Config.visuals.offscreen_esp;
  nadeLineups = g_Config.visuals.nade_lineups;
  bulletTracer = g_Config.visuals.bullet_tracers;
  teamCheck = g_Config.visuals.enemies;
  drawAmmoBar = g_Config.visuals.ammo_bar;
  drawSnaplines = g_Config.visuals.snaplines;
  drawEyeRay = g_Config.visuals.eye_ray;
  drawDistance = g_Config.visuals.distance;
  drawFlags = g_Config.visuals.flags;
  chamsEnabled = g_Config.visuals.chams_enabled;
  chamsType = g_Config.visuals.chams_type;
  drawSkeletons = g_Config.skeleton.enabled;
  skeletonStyle = g_Config.skeleton.mode;
  g_Config.skeleton.head_circle = g_Config.visuals.head_pos;
  drawHeadDot = g_Config.visuals.head_pos;
  showSpectators = g_Config.spectators.enabled;
  g_Config.other_assists.fov_circle = g_Config.aimbot.draw_fov;
  aimbotFovCircle = g_Config.aimbot.draw_fov;
  aimbotLead = wcfg.target_lead;
}

// ============ NEWGUI RENDERING (unchanged, kept for full fidelity)
// ============
void DrawSpringLogo(ImDrawList *draw, ImVec2 pos, float size) {
  float s = size / 64.0f;
  ImU32 stroke_1a = g_Config.is_dark_mode ? IM_COL32(245, 245, 250, 255)
                                          : IM_COL32(26, 26, 26, 255);
  ImU32 stroke_2a = g_Config.is_dark_mode ? IM_COL32(215, 215, 225, 255)
                                          : IM_COL32(42, 42, 42, 255);
  ImU32 stroke_44 = g_Config.is_dark_mode ? IM_COL32(185, 185, 195, 255)
                                          : IM_COL32(68, 68, 68, 255);
  auto P = [&](float x, float y) {
    return ImVec2(pos.x + x * s, pos.y + y * s);
  };
  draw->AddBezierCubic(P(16, 48), P(18, 36), P(20, 22), P(32, 16), stroke_1a,
                       2.5f * s);
  draw->AddBezierCubic(P(32, 16), P(44, 22), P(46, 36), P(48, 48), stroke_1a,
                       2.5f * s);
  draw->AddBezierCubic(P(20, 18), P(26, 14), P(38, 14), P(44, 18), stroke_1a,
                       2.5f * s);
  draw->AddBezierCubic(P(24, 24), P(24, 24), P(27, 34), P(26, 40), stroke_2a,
                       2.0f * s);
  draw->AddBezierCubic(P(40, 24), P(40, 24), P(37, 34), P(38, 40), stroke_2a,
                       2.0f * s);
  draw->AddCircleFilled(P(27, 33), 2.2f * s, stroke_1a);
  draw->AddCircleFilled(P(37, 33), 2.2f * s, stroke_1a);
  draw->AddBezierCubic(P(30, 40), P(31, 41), P(33, 41), P(34, 40), stroke_44,
                       1.5f * s);
}

void DrawSidebarIcon(ImDrawList *draw, int icon_type, ImVec2 center,
                     ImU32 col) {
  const float th = 1.2f;
  switch (icon_type) {
  case 0: {
    draw->AddCircle(center, 5.0f, col, 24, th);
    draw->AddLine({center.x - 7.5f, center.y}, {center.x - 4.2f, center.y}, col,
                  th);
    draw->AddLine({center.x + 4.2f, center.y}, {center.x + 7.5f, center.y}, col,
                  th);
    draw->AddLine({center.x, center.y - 7.5f}, {center.x, center.y - 4.2f}, col,
                  th);
    draw->AddLine({center.x, center.y + 4.2f}, {center.x, center.y + 7.5f}, col,
                  th);
  } break;
  case 1: {
    draw->AddCircle(center, 5.5f, col, 16, th);
    draw->AddCircleFilled(center, 1.8f, col);
  } break;
  case 2: {
    draw->AddLine({center.x - 4.5f, center.y - 5.5f},
                  {center.x - 4.5f, center.y + 5.5f}, col, th);
    draw->AddLine({center.x - 6.0f, center.y + 1.5f},
                  {center.x - 3.0f, center.y + 1.5f}, col, 1.6f);
    draw->AddLine({center.x, center.y - 5.5f}, {center.x, center.y + 5.5f}, col,
                  th);
    draw->AddLine({center.x - 1.5f, center.y - 2.5f},
                  {center.x + 1.5f, center.y - 2.5f}, col, 1.6f);
    draw->AddLine({center.x + 4.5f, center.y - 5.5f},
                  {center.x + 4.5f, center.y + 5.5f}, col, th);
    draw->AddLine({center.x + 3.0f, center.y + 0.5f},
                  {center.x + 6.0f, center.y + 0.5f}, col, 1.6f);
  } break;
  case 3: {
    draw->PathArcTo({center.x, center.y - 1.2f}, 6.5f, 0.40f, 2.74f, 16);
    draw->PathStroke(col, 0, th);
    draw->PathArcTo({center.x, center.y + 1.2f}, 6.5f, 3.54f, 5.88f, 16);
    draw->PathStroke(col, 0, th);
    draw->AddCircle(center, 1.8f, col, 16, th);
  } break;
  case 4: {
    ImVec2 pts[] = {
        {center.x + 1.5f, center.y - 6.5f}, {center.x - 3.5f, center.y - 0.5f},
        {center.x + 0.5f, center.y - 0.5f}, {center.x - 1.5f, center.y + 6.5f},
        {center.x + 3.5f, center.y + 0.5f}, {center.x - 0.5f, center.y + 0.5f}};
    draw->AddPolyline(pts, 6, col, ImDrawFlags_Closed, th);
  } break;
  case 5: {
    draw->PathArcTo({center.x, center.y - 1.2f}, 6.5f, 0.40f, 2.74f, 16);
    draw->PathStroke(col, 0, th);
    draw->PathArcTo({center.x, center.y + 1.2f}, 6.5f, 3.54f, 5.88f, 16);
    draw->PathStroke(col, 0, th);
    draw->AddCircle(center, 1.8f, col, 16, th);
  } break;
  case 6: {
    ImVec2 p_pts[] = {
        {center.x + 4.8f, center.y - 2.2f}, {center.x + 2.8f, center.y - 5.2f},
        {center.x - 2.2f, center.y - 5.2f}, {center.x - 5.2f, center.y - 2.2f},
        {center.x - 5.2f, center.y + 2.2f}, {center.x - 2.2f, center.y + 5.2f},
        {center.x + 1.8f, center.y + 4.8f}, {center.x + 3.8f, center.y + 2.8f},
        {center.x + 2.2f, center.y + 1.0f}, {center.x + 3.2f, center.y - 0.5f},
        {center.x + 5.2f, center.y - 0.5f}};
    draw->AddPolyline(p_pts, 11, col, ImDrawFlags_Closed, th);
    draw->AddCircle({center.x + 1.6f, center.y + 2.4f}, 1.0f, col, 10, th);
    draw->AddCircleFilled({center.x - 2.6f, center.y - 2.5f}, 0.85f, col);
    draw->AddCircleFilled({center.x + 0.6f, center.y - 3.2f}, 0.85f, col);
    draw->AddCircleFilled({center.x - 3.0f, center.y + 1.6f}, 0.85f, col);
  } break;
  case 7: {
    draw->PathArcTo(center, 5.2f, 3.14159f * 1.05f, 3.14159f * 1.95f, 16);
    draw->PathStroke(col, 0, th);
    draw->AddLine({center.x + 5.2f, center.y - 3.5f},
                  {center.x + 5.2f, center.y + 0.2f}, col, th);
    draw->AddLine({center.x + 2.0f, center.y - 0.2f},
                  {center.x + 5.2f, center.y + 0.2f}, col, th);
    draw->PathArcTo(center, 5.2f, 3.14159f * 0.05f, 3.14159f * 0.95f, 16);
    draw->PathStroke(col, 0, th);
    draw->AddLine({center.x - 5.2f, center.y + 3.5f},
                  {center.x - 5.2f, center.y - 0.2f}, col, th);
    draw->AddLine({center.x - 2.0f, center.y + 0.2f},
                  {center.x - 5.2f, center.y - 0.2f}, col, th);
  } break;
  case 8: {
    draw->AddRect({center.x - 5.5f, center.y - 2.0f},
                  {center.x + 5.5f, center.y + 6.2f}, col, 1.8f, 0, th);
    draw->AddLine({center.x - 4.0f, center.y - 5.5f},
                  {center.x - 5.5f, center.y - 2.0f}, col, th);
    draw->AddLine({center.x + 4.0f, center.y - 5.5f},
                  {center.x + 5.5f, center.y - 2.0f}, col, th);
    draw->AddLine({center.x - 4.0f, center.y - 5.5f},
                  {center.x + 4.0f, center.y - 5.5f}, col, th);
    draw->PathArcTo({center.x, center.y - 2.0f}, 2.2f, 0.0f, 3.14159f, 12);
    draw->PathStroke(col, 0, th);
  } break;
  case 9: {
    float r_out = 6.2f, r_mid = 4.6f, r_hole = 2.0f;
    const int teeth = 8;
    for (int i = 0; i < teeth; i++) {
      float a = i * (6.283185f / teeth);
      draw->AddLine({center.x + cosf(a) * r_mid, center.y + sinf(a) * r_mid},
                    {center.x + cosf(a) * r_out, center.y + sinf(a) * r_out},
                    col, 1.3f);
    }
    draw->AddCircle(center, r_mid, col, 24, th);
    draw->AddCircle(center, r_hole, col, 16, th);
  } break;
  default:
    draw->AddCircle(center, 4.5f, col, 16, th);
    break;
  }
}

void DrawHeaderActionIcons(ImDrawList *draw, ImVec2 area_pos, float area_w,
                           float area_h) {
  auto &pal = Theme::Get(g_Config.is_dark_mode);
  ImU32 icon_col = Theme::ToU32(pal.TextSecondary);
  float right_x = area_pos.x + area_w - 18.0f;
  float center_y = area_pos.y + area_h * 0.5f;

  float close_x = right_x - 10.0f;
  ImGui::SetCursorScreenPos({close_x - 10.0f, center_y - 10.0f});
  if (ImGui::InvisibleButton("##hdr_close", {20.0f, 20.0f}))
    PostQuitMessage(0);
  bool close_hov = ImGui::IsItemHovered();
  ImU32 c_col = close_hov ? IM_COL32(239, 68, 68, 255) : icon_col;
  draw->AddLine({close_x - 4.5f, center_y - 4.5f},
                {close_x + 4.5f, center_y + 4.5f}, c_col, 1.8f);
  draw->AddLine({close_x + 4.5f, center_y - 4.5f},
                {close_x - 4.5f, center_y + 4.5f}, c_col, 1.8f);

  float sun_x = close_x - 30.0f;
  ImGui::SetCursorScreenPos({sun_x - 10.0f, center_y - 10.0f});
  if (ImGui::InvisibleButton("##hdr_theme", {20.0f, 20.0f})) {
    g_Config.is_dark_mode = !g_Config.is_dark_mode;
    Theme::SetupImGuiStyle(g_Config.is_dark_mode);
    ReactiveState::DispatchToggle(FEATURE_INTERFACE_DARK_MODE,
                                  g_Config.is_dark_mode);
  }
  bool sun_hov = ImGui::IsItemHovered();
  ImU32 s_col = sun_hov ? Theme::ToU32(pal.TextPrimary) : icon_col;
  if (g_Config.is_dark_mode) {
    const int MOON_SEGS = 16;
    draw->PathClear();
    for (int i = 0; i <= MOON_SEGS; i++) {
      float a = 0.85f + (4.71f - 0.85f) * ((float)i / (float)MOON_SEGS);
      draw->PathLineTo({sun_x + 5.2f * cosf(a), center_y + 5.2f * sinf(a)});
    }
    for (int i = 0; i <= MOON_SEGS; i++) {
      float a = 4.71f + (0.85f - 4.71f) * ((float)i / (float)MOON_SEGS);
      draw->PathLineTo(
          {sun_x + 2.4f + 4.2f * cosf(a), center_y - 2.4f + 4.2f * sinf(a)});
    }
    draw->PathStroke(s_col, ImDrawFlags_Closed, 1.4f);
  } else {
    draw->AddCircle({sun_x, center_y}, 4.5f, s_col, 16, 1.4f);
    for (int a = 0; a < 8; a++) {
      float rad = a * 3.14159f / 4.0f;
      draw->AddLine({sun_x + cosf(rad) * 6.5f, center_y + sinf(rad) * 6.5f},
                    {sun_x + cosf(rad) * 8.5f, center_y + sinf(rad) * 8.5f},
                    s_col, 1.2f);
    }
  }

  float search_x = sun_x - 30.0f;
  ImGui::SetCursorScreenPos({search_x - 10.0f, center_y - 10.0f});
  if (ImGui::InvisibleButton("##hdr_search", {20.0f, 20.0f}))
    g_Config.search_open = !g_Config.search_open;
  bool search_hov = ImGui::IsItemHovered();
  ImU32 q_col = search_hov ? Theme::ToU32(pal.TextPrimary) : icon_col;
  draw->AddCircle({search_x - 1.5f, center_y - 1.5f}, 4.5f, q_col, 16, 1.4f);
  draw->AddLine({search_x + 1.5f, center_y + 1.5f},
                {search_x + 5.5f, center_y + 5.5f}, q_col, 1.6f);
}

void DrawCutGlassPane(ImDrawList *draw, ImVec2 p_min, ImVec2 p_max, float r,
                      bool is_dark) {
  float h = p_max.y - p_min.y;
  if (r < 0.0f || r > h * 0.5f)
    r = h * 0.5f;
  draw->AddRectFilled(
      {p_min.x + 0.5f, p_min.y + 1.5f}, {p_max.x - 0.5f, p_max.y + 2.5f},
      is_dark ? IM_COL32(0, 0, 0, 35) : IM_COL32(0, 0, 0, 14), r);
  ImU32 col_bg =
      is_dark ? IM_COL32(42, 46, 56, 245) : IM_COL32(220, 225, 235, 245);
  draw->AddRectFilled(p_min, p_max, col_bg, r);
  draw->AddLine({p_min.x + r, p_min.y + 0.5f}, {p_max.x - r, p_min.y + 0.5f},
                is_dark ? IM_COL32(255, 255, 255, 35)
                        : IM_COL32(255, 255, 255, 140),
                1.0f);
}

ID3D11ShaderResourceView* CreateTextureFromMemory(ID3D11Device* device, const unsigned char* data, size_t sz) {
  if (!device || !data || sz == 0) return nullptr;
  int w = 0, h = 0, channels = 0;
  unsigned char* pixels = stbi_load_from_memory(data, (int)sz, &w, &h, &channels, 4);
  if (!pixels) return nullptr;

  D3D11_TEXTURE2D_DESC desc{};
  desc.Width = w;
  desc.Height = h;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  D3D11_SUBRESOURCE_DATA subData{};
  subData.pSysMem = pixels;
  subData.SysMemPitch = w * 4;

  ID3D11Texture2D* pTexture = nullptr;
  HRESULT hr = device->CreateTexture2D(&desc, &subData, &pTexture);
  stbi_image_free(pixels);
  if (FAILED(hr) || !pTexture) return nullptr;

  ID3D11ShaderResourceView* srv = nullptr;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
  srvDesc.Format = desc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  hr = device->CreateShaderResourceView(pTexture, &srvDesc, &srv);
  pTexture->Release();
  return FAILED(hr) ? nullptr : srv;
}

void RenderMainWindow() {
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigWindowsResizeFromEdges = true;
  io.MouseDrawCursor = true;

  // Apply theme palette and opacity dynamically
  Theme::SetupImGuiStyle(g_Config.is_dark_mode, g_Config.menu_opacity);
  const auto &pal = Theme::Get(g_Config.is_dark_mode);

  c::anim::speed = ImClamp(io.DeltaTime * 14.f, 0.01f, 1.0f);
  c::second_color = utils::GetDarkColor(c::main_color);

  // Sync background colors to active dark/light mode and user opacity
  float menu_alpha = ImClamp(g_Config.menu_opacity, 0.20f, 1.0f);
  if (g_Config.is_dark_mode) {
    c::window_bg_color = rgba(20, 22, 28, menu_alpha);
    c::child::background = rgba(30, 33, 42, menu_alpha * 0.75f);
    c::label::active = rgba(245, 245, 252, 1.0f);
    c::label::def = rgba(160, 165, 185, 0.9f);
  } else {
    c::window_bg_color = rgba(242, 244, 248, menu_alpha);
    c::child::background = rgba(225, 228, 236, menu_alpha * 0.85f);
    c::label::active = rgba(25, 28, 36, 1.0f);
    c::label::def = rgba(100, 105, 120, 0.9f);
  }

  ImVec2 display_sz = io.DisplaySize;
  ImVec2 win_sz = c::bg::size;
  ImVec2 target_pos((display_sz.x - win_sz.x) * 0.5f, (display_sz.y - win_sz.y) * 0.5f);
  if (target_pos.x < 10.f) target_pos.x = 10.f;
  if (target_pos.y < 10.f) target_pos.y = 10.f;

  ImGui::SetNextWindowPos(target_pos, ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(win_sz, ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(ImVec2(660.0f, 440.0f), ImVec2(3840.0f, 2160.0f));

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoBackground;

  if (ImGui::Begin("##LianFlowMainWindow", nullptr, flags)) {
    const ImVec2 curWinSize = ImGui::GetWindowSize();
    const ImVec2 pos = ImGui::GetWindowPos();

    // Top header drag handle (covers header center, leaving margins for window edge/corner resizing)
    ImGui::SetCursorPos(ImVec2(14.0f, 10.0f));
    ImGui::InvisibleButton("##HeaderDragBar", ImVec2(std::max(100.0f, curWinSize.x - 140.0f), 52.0f));
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      ImVec2 delta = io.MouseDelta;
      if (delta.x != 0.0f || delta.y != 0.0f) {
        ImGui::SetWindowPos("##LianFlowMainWindow", ImVec2(pos.x + delta.x, pos.y + delta.y));
      }
    }

    ImDrawList *bgDl = ImGui::GetBackgroundDrawList();

    // Main window background with opacity applied
    bgDl->AddRectFilled(
        pos, pos + curWinSize,
        utils::GetColorWithAlpha(c::window_bg_color,
                                 c::window_bg_color.Value.w),
        c::bg::rounding);

    // Outer border highlight matching current theme
    ImU32 borderCol = g_Config.is_dark_mode ? IM_COL32(255, 255, 255, (int)(32 * menu_alpha))
                                            : IM_COL32(0, 0, 0, (int)(28 * menu_alpha));
    bgDl->AddRect(
        pos, pos + curWinSize,
        borderCol,
        c::bg::rounding, 0, 1.2f);

    // Sidebar header accent banner
    bgDl->AddRectFilled(
        pos, pos + ImVec2(curWinSize.x, 70), GetColorU32(c::child::background),
        c::bg::rounding, ImDrawFlags_RoundCornersTopRight);

    // Header Logo & Title
    if (font::bold_font)
      ImGui::PushFont(font::bold_font);
    bgDl->AddText(
        utils::center_text(pos, pos + ImVec2(70, 70), ICON_FIRE_FILL) +
            ImVec2(0, 4.5f),
        c::main_color, ICON_FIRE_FILL);
    bgDl->AddText(
        ImVec2(pos.x + 60,
               utils::center_text(pos, pos + ImVec2(70, 70), "LianFlow").y),
        c::label::active, "LianFlow");
    if (font::bold_font)
      ImGui::PopFont();

    // Header action buttons (Theme Dark/Light toggle, Search toggle, Close button)
    DrawHeaderActionIcons(bgDl, pos, curWinSize.x, 70.0f);

    // Top Right resolution badge
    char sizeBuf[64];
    snprintf(sizeBuf, sizeof(sizeBuf), "%dx%d", (int)curWinSize.x, (int)curWinSize.y);
    float badge_x = pos.x + curWinSize.x - 120.0f - ImGui::CalcTextSize(sizeBuf).x;
    bgDl->AddText(ImVec2(badge_x, pos.y + 26.0f),
                  g_Config.is_dark_mode ? IM_COL32(255, 255, 255, 90) : IM_COL32(30, 30, 40, 120),
                  sizeBuf);

    // Dynamic vertical divider line between sidebar and content cards
    ImU32 dividerCol = g_Config.is_dark_mode ? IM_COL32(255, 255, 255, 18) : IM_COL32(0, 0, 0, 20);
    bgDl->AddLine(
        ImVec2(pos.x + 192.0f, pos.y + 82.0f),
        ImVec2(pos.x + 192.0f, pos.y + curWinSize.y - 20.0f),
        dividerCol, 1.0f);

    // Footer build status text
    bgDl->AddText(
        pos + ImVec2(15, curWinSize.y - 28), c::label::def,
        "CS2 Build 14185");

    // Bottom-right corner resize grip indicator
    ImVec2 br = pos + curWinSize;
    ImU32 gripCol = g_Config.is_dark_mode ? IM_COL32(255, 255, 255, 80) : IM_COL32(50, 50, 60, 90);
    bgDl->AddLine(ImVec2(br.x - 14.0f, br.y - 5.0f), ImVec2(br.x - 5.0f, br.y - 14.0f), gripCol, 1.5f);
    bgDl->AddLine(ImVec2(br.x - 10.0f, br.y - 5.0f), ImVec2(br.x - 5.0f, br.y - 10.0f), gripCol, 1.5f);
    bgDl->AddLine(ImVec2(br.x - 6.0f, br.y - 5.0f), ImVec2(br.x - 5.0f, br.y - 6.0f), gripCol, 1.5f);

    static std::vector<s_tab> tabs_info = {
        {"Combat", {"Aimbot Config", "Trigger & Recoil"}},
        {"Visuals", {"Player ESP", "Chams & Radar"}},
        {"World", {"Grenades & Lineups", "Bomb & Loot"}},
        {"Settings", {"Profiles & Keybinds", "Configuration"}}};

    static c_tabs p_tabs(tabs_info);

    // Custom responsive sidebar rendering
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::SetCursorPos(ImVec2(18.f, 82.f));

    float sidebar_h = std::max(200.0f, curWinSize.y - 120.0f);
    if (ImGui::BeginChild("##ResponsiveTabs", ImVec2(168.f, sidebar_h), false,
                          ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav)) {
      int check_id = 0;
      for (size_t n = 0; n < tabs_info.size(); n++) {
        if (n > 0) ImGui::Dummy(ImVec2(0, 8.0f));
        std::string upper_name = tabs_info[n].header;
        for (char &ch : upper_name) ch = (char)toupper((unsigned char)ch);

        ImFont *h_font = font::s_inter_semibold ? font::s_inter_semibold :
                         (font::inter_medium ? font::inter_medium : ImGui::GetFont());
        if (h_font) ImGui::PushFont(h_font);
        ImVec2 curHPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddCircleFilled(
            ImVec2(curHPos.x + 3.0f, curHPos.y + 7.5f), 2.2f,
            ImColor(c::main_color.Value.x, c::main_color.Value.y, c::main_color.Value.z, 0.85f));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
        ImVec4 headerTextCol = g_Config.is_dark_mode ? ImVec4(0.70f, 0.67f, 0.80f, 0.85f)
                                                     : ImVec4(0.35f, 0.38f, 0.48f, 0.95f);
        ImGui::TextColored(headerTextCol, "%s", upper_name.c_str());
        if (h_font) ImGui::PopFont();
        ImGui::Spacing();

        for (size_t i = 0; i < tabs_info[n].tabs.size(); i++) {
          custom::Tab(tabs_info[n].tabs[i], (int*)&p_tabs, check_id);
          check_id++;
        }
      }
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

    // Dynamically calculate content panel width and height based on resized window
    float content_x = 208.0f;
    float margin_right = 24.0f;
    float margin_bottom = 24.0f;
    float content_y = 85.0f;

    float avail_w = std::max(280.0f, curWinSize.x - content_x - margin_right);
    float col_gap = 16.0f;
    float card_w = (avail_w - col_gap) * 0.5f;

    float avail_h = std::max(180.0f, curWinSize.y - content_y - margin_bottom);
    float card_content_h = avail_h - 40.0f;

    // SUBTAB 0: Aimbot Config
    if (p_tabs.IsTabActive(0)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Aimbot Setup##L0", ImVec2(card_w, card_content_h), true);
      {
        int activeWpnIdx = (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT) ? g_Config.active_weapon : WEAPON_GENERAL;
        if (custom::Checkbox("Enable Aimbot", &g_Config.aimbot.enabled)) {
          g_Config.weapons[activeWpnIdx].enabled = g_Config.aimbot.enabled;
          g_Config.weapons[WEAPON_GENERAL].enabled = g_Config.aimbot.enabled;
          aimbotEnabled = g_Config.aimbot.enabled;
        }

        static const char *aim_keys[] = {"Left Alt", "Mouse 4", "Mouse 5", "Left Shift", "Left Click", "Auto"};
        static int curAimKeyIdx = 0;
        if (custom::Combo("Aimbot Key", &curAimKeyIdx, aim_keys, IM_ARRAYSIZE(aim_keys))) {
          if (curAimKeyIdx == 0) g_aimKeyVk = VK_LMENU;
          else if (curAimKeyIdx == 1) g_aimKeyVk = VK_XBUTTON1;
          else if (curAimKeyIdx == 2) g_aimKeyVk = VK_XBUTTON2;
          else if (curAimKeyIdx == 3) g_aimKeyVk = VK_LSHIFT;
          else if (curAimKeyIdx == 4) g_aimKeyVk = VK_LBUTTON;
          else g_aimKeyVk = -1;
        }

        static const char *target_modes[] = {"Distance", "FOV Angle", "Lowest Health"};
        custom::Combo("Target Priority", &g_Config.aimbot.target_mode, target_modes, IM_ARRAYSIZE(target_modes));

        static const char *aim_bones[] = {"Head", "Neck", "Chest", "Pelvis"};
        custom::Combo("Target Bone", &g_Config.aimbot.target_bone, aim_bones, IM_ARRAYSIZE(aim_bones));

        int fovInt = (int)g_Config.aimbot.fov;
        if (custom::SliderInt("FOV Radius", &fovInt, 1, 45)) {
          g_Config.aimbot.fov = (float)fovInt;
          aimbotFovDeg = (float)fovInt;
          g_Config.weapons[activeWpnIdx].fov = (float)fovInt;
        }

        int smoothInt = (int)g_Config.aimbot.smooth;
        if (custom::SliderInt("Smooth Factor", &smoothInt, 1, 50)) {
          g_Config.aimbot.smooth = (float)smoothInt;
          aimbotSmooth = (float)smoothInt;
          g_Config.weapons[activeWpnIdx].smooth_strength = (float)smoothInt;
        }
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Aimbot Automation##R0", ImVec2(card_w, card_content_h), true);
      {
        int activeWpnIdx = (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT) ? g_Config.active_weapon : WEAPON_GENERAL;
        if (custom::Checkbox("Draw FOV Circle", &g_Config.aimbot.draw_fov)) {
          g_Config.other_assists.fov_circle = g_Config.aimbot.draw_fov;
          aimbotFovCircle = g_Config.aimbot.draw_fov;
        }
        custom::ColorEdit4("FOV Circle Color", g_Config.aimbot.fov_color);

        if (custom::Checkbox("Visibility Check", &g_Config.aimbot.vis_check)) {
          g_Config.weapons[activeWpnIdx].cond_visible = g_Config.aimbot.vis_check;
          aimbotWallCheck = g_Config.aimbot.vis_check;
        }
        if (custom::Checkbox("Target Lead / Prediction", &g_Config.weapons[activeWpnIdx].target_lead)) {
          aimbotLead = g_Config.weapons[activeWpnIdx].target_lead;
        }
        custom::Checkbox("Auto Fire / Auto Shoot", &g_Config.aimbot.auto_fire);

        custom::Separator();
        custom::Checkbox("Recoil Crosshair", &g_Config.visuals.recoil_crosshair);
        custom::Checkbox("Sniper Crosshair", &g_Config.visuals.sniper_crosshair);
      }
      custom::EndChild();
    }

    // SUBTAB 1: Trigger & Recoil
    if (p_tabs.IsTabActive(1)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Triggerbot Settings##L1", ImVec2(card_w, card_content_h), true);
      {
        int activeWpnIdx = (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT) ? g_Config.active_weapon : WEAPON_GENERAL;
        if (custom::Checkbox("Enable Triggerbot", &g_Config.triggerbot.enabled)) {
          g_Config.weapons[activeWpnIdx].trigger_enabled = g_Config.triggerbot.enabled;
          triggerbotEnabled = g_Config.triggerbot.enabled;
        }

        custom::SliderInt("Trigger Delay (ms)", &g_Config.triggerbot.delay_ms, 0, 200);
        triggerDelay = g_Config.triggerbot.delay_ms;

        static const char *trig_keys[] = {"Mouse 4", "Mouse 5", "Left Alt", "Left Shift"};
        static int curTrigKeyIdx = 0;
        if (custom::Combo("Trigger Key", &curTrigKeyIdx, trig_keys, IM_ARRAYSIZE(trig_keys))) {
          if (curTrigKeyIdx == 0) g_triggerKeyVk = VK_XBUTTON1;
          else if (curTrigKeyIdx == 1) g_triggerKeyVk = VK_XBUTTON2;
          else if (curTrigKeyIdx == 2) g_triggerKeyVk = VK_LMENU;
          else g_triggerKeyVk = VK_LSHIFT;
        }

        custom::Separator();
        custom::Checkbox("Auto Pistol", &g_Config.other_assists.auto_pistol);
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Recoil & Assist Control##R1", ImVec2(card_w, card_content_h), true);
      {
        int activeWpnIdx = (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT) ? g_Config.active_weapon : WEAPON_GENERAL;
        if (custom::Checkbox("Auto Stop / Quick Stop", &g_Config.weapons[activeWpnIdx].auto_stop)) {
          autoStopEnabled = g_Config.weapons[activeWpnIdx].auto_stop;
        }
        custom::Checkbox("Master Panic Key (])", &g_masterEnabled);

        custom::Separator();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.75f, 0.9f), "Trigger activates instant shot on crosshair trace.");
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.75f, 0.9f), "Auto Stop decelerates player for accurate first-shot accuracy.");
      }
      custom::EndChild();
    }

    // SUBTAB 2: Player ESP
    if (p_tabs.IsTabActive(2)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Bounding Boxes & Vitals##L2", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Enable Visuals", &g_Config.visuals.enabled)) {
          espEnabled = g_Config.visuals.enabled;
        }
        if (custom::Checkbox("Enemies Only", &g_Config.visuals.enemies)) {
          teamCheck = g_Config.visuals.enemies;
        }

        if (custom::Checkbox("2D Bounding Box", &g_Config.visuals.box)) {
          g_Config.visuals.box_esp = g_Config.visuals.box;
          drawBoxes = g_Config.visuals.box;
        }
        custom::Checkbox("Box Outline", &g_Config.visuals.box_outline);
        custom::Checkbox("Box Filled", &g_Config.visuals.box_fill);

        int boxThick = (int)g_Config.visuals.box_thickness;
        if (custom::SliderInt("Box Thickness", &boxThick, 1, 8))
          g_Config.visuals.box_thickness = (float)boxThick;

        if (custom::Checkbox("Health Bar", &g_Config.visuals.health)) {
          g_Config.visuals.health_bar = g_Config.visuals.health;
          drawHealth = g_Config.visuals.health;
        }

        static const char *hp_positions[] = {"Left Bar", "Top Bar", "Bottom Bar"};
        custom::Combo("Health Bar Position", &g_Config.visuals.esp_hp_pos, hp_positions, IM_ARRAYSIZE(hp_positions));

        if (custom::Checkbox("Armor Indicator", &g_Config.visuals.armor)) {
          g_Config.visuals.armor_bar = g_Config.visuals.armor;
          drawArmor = g_Config.visuals.armor;
        }
        if (custom::Checkbox("Ammo Bar", &g_Config.visuals.ammo_bar)) {
          drawAmmoBar = g_Config.visuals.ammo_bar;
        }
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Player Tags & Info##R2", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Player Names", &g_Config.visuals.names)) {
          g_Config.visuals.player_names = g_Config.visuals.names;
          drawNames = g_Config.visuals.names;
        }
        if (custom::Checkbox("Distance Tag", &g_Config.visuals.distance)) {
          drawDistance = g_Config.visuals.distance;
        }
        if (custom::Checkbox("Active Weapon Icon", &g_Config.visuals.weapon)) {
          g_Config.visuals.weapon_icon = g_Config.visuals.weapon;
          drawWeapon = g_Config.visuals.weapon;
        }
        if (custom::Checkbox("Status Flags (Defuse/Plant/Flash)", &g_Config.visuals.flags)) {
          drawFlags = g_Config.visuals.flags;
        }

        custom::Separator();
        if (custom::Checkbox("Offscreen Radar Arrows", &g_Config.visuals.offscreen_esp)) {
          offscreenEsp = g_Config.visuals.offscreen_esp;
        }
        if (custom::Checkbox("Sound ESP Footsteps", &g_Config.visuals.sound_esp)) {
          soundEsp = g_Config.visuals.sound_esp;
        }
      }
      custom::EndChild();
    }

    // SUBTAB 3: Chams & Radar
    if (p_tabs.IsTabActive(3)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Skeleton & Overlays##L3", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Player Skeleton", &g_Config.skeleton.enabled)) {
          drawSkeletons = g_Config.skeleton.enabled;
        }

        int skelThick = (int)g_Config.skeleton.thickness;
        if (custom::SliderInt("Skeleton Thickness", &skelThick, 1, 8))
          g_Config.skeleton.thickness = (float)skelThick;

        custom::Checkbox("Show All Joint Dots", &g_Config.skeleton.joints);
        if (custom::Checkbox("Head Position Circle", &g_Config.visuals.head_pos)) {
          g_Config.skeleton.head_circle = g_Config.visuals.head_pos;
          drawHeadDot = g_Config.visuals.head_pos;
        }
        if (custom::Checkbox("Eye Ray Aim Line", &g_Config.visuals.eye_ray)) {
          drawEyeRay = g_Config.visuals.eye_ray;
        }
        if (custom::Checkbox("Snaplines to Feet", &g_Config.visuals.snaplines)) {
          drawSnaplines = g_Config.visuals.snaplines;
        }
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Color Palettes & Shading##R3", ImVec2(card_w, card_content_h), true);
      {
        custom::ColorEdit4("Enemy Visible Color", g_Config.visuals.box_visible_color);
        custom::ColorEdit4("Enemy Occluded Color", g_Config.visuals.box_color);
        custom::ColorEdit4("Skeleton Color", g_Config.skeleton.color);
        custom::ColorEdit4("Head Dot Color", g_Config.visuals.head_pos_color);
        custom::ColorEdit4("Snaplines Color", g_Config.visuals.snaplines_color);
      }
      custom::EndChild();
    }

    // SUBTAB 4: Grenades & Lineups
    if (p_tabs.IsTabActive(4)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Live Grenades & Trajectory##L4", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Live Grenade ESP", &g_Config.visuals.grenades_esp)) {
          liveNades = g_Config.visuals.grenades_esp;
        }
        if (custom::Checkbox("Trajectory Prediction", &g_Config.visuals.grenade_trajectory)) {
          nadeTrajectory = g_Config.visuals.grenade_trajectory;
        }
        if (custom::Checkbox("Grenade Lineup Helper", &g_Config.visuals.nade_lineups)) {
          nadeLineups = g_Config.visuals.nade_lineups;
        }
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Lineup Spots & Map Select##R4", ImVec2(card_w, card_content_h), true);
      {
        static const char *maps[] = {"Mirage", "Inferno", "Nuke", "Overpass", "Dust II", "Ancient", "Anubis"};
        custom::Combo("Current Map", &selectedMapIdx, maps, IM_ARRAYSIZE(maps));

        static const char *lineup_modes[] = {"Nearby Spots Only", "Show All Spots"};
        custom::Combo("Lineup Filter", &lineupShowAllMode, lineup_modes, IM_ARRAYSIZE(lineup_modes));

        custom::Separator();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.75f, 0.9f), "Overlay renders precise crosshair position ring");
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.75f, 0.9f), "and throw method indicator (Jumpthrow/Run).");
      }
      custom::EndChild();
    }

    // SUBTAB 5: Bomb & Loot
    if (p_tabs.IsTabActive(5)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Bomb & Objective Tracker##L5", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Planted C4 Countdown Timer", &g_Config.visuals.bomb_esp)) {
          bombEsp = g_Config.visuals.bomb_esp;
        }
        if (custom::Checkbox("Dropped C4 Carrier ESP", &g_Config.visuals.bomb_carrier)) {
          droppedC4Esp = g_Config.visuals.bomb_carrier;
        }
        if (custom::Checkbox("Spectator List Window", &g_Config.spectators.enabled)) {
          showSpectators = g_Config.spectators.enabled;
        }
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("World Loot & Bullet Tracers##R5", ImVec2(card_w, card_content_h), true);
      {
        if (custom::Checkbox("Dropped Weapons / Loot ESP", &g_Config.visuals.dropped_weapons)) {
          worldLoot = g_Config.visuals.dropped_weapons;
        }
        if (custom::Checkbox("Bullet Tracers", &g_Config.visuals.bullet_tracers)) {
          bulletTracer = g_Config.visuals.bullet_tracers;
        }
        custom::ColorEdit4("Bullet Tracer Color", bulletColor);
      }
      custom::EndChild();
    }

    // SUBTAB 6: Profiles & Keybinds
    if (p_tabs.IsTabActive(6)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Weapon Class Profiles##L6", ImVec2(card_w, card_content_h), true);
      {
        static const char *weapon_profiles[] = {"General", "Pistol", "Rifle", "Sniper", "SMG", "Shotgun"};
        custom::Combo("Active Profile", &g_Config.active_weapon, weapon_profiles, IM_ARRAYSIZE(weapon_profiles));
        custom::Checkbox("Auto Weapon Switching", &g_Config.auto_weapon_detect);
        custom::Checkbox("Dynamic Island HUD", &g_Config.island.enabled);
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Keybind Overview##R6", ImVec2(card_w, card_content_h), true);
      {
        ImGui::TextColored(ImVec4(0.85f, 0.85f, 0.9f, 0.95f), "Active Keybind Mapping:");
        ImGui::Spacing();
        ImGui::BulletText("Aimbot: Left Alt / Mouse 4 / Shift");
        ImGui::BulletText("Triggerbot: Mouse 4 / Mouse 5");
        ImGui::BulletText("Master Panic Key: ]");
        ImGui::BulletText("Menu Toggle: Insert");
      }
      custom::EndChild();
    }

    // SUBTAB 7: Configuration
    if (p_tabs.IsTabActive(7)) {
      ImGui::SetCursorPos(ImVec2(content_x, content_y));
      custom::Child("Menu Appearance & Theme##L7", ImVec2(card_w, card_content_h), true);
      {
        custom::ColorEdit4("Menu Accent Color", (float *)&c::main_color);
        custom::Checkbox("Dark Mode Theme", &g_Config.is_dark_mode);
        custom::SliderFloat("Menu Opacity", &g_Config.menu_opacity, 0.20f, 1.0f, "%.2f");
        custom::SliderInt("Overlay FPS Limit", &g_Config.max_fps, 30, 360);
      }
      custom::EndChild();

      ImGui::SameLine(0, col_gap);

      custom::Child("Save & Load Configurations##R7", ImVec2(card_w, card_content_h), true);
      {
        custom::Checkbox("Auto-Save Config on Exit", &autoSaveConfig);

        custom::Separator();
        if (custom::Button("Save Settings", ImVec2(150, 38))) {
          SaveConfig("config.cfg");
        }
        ImGui::SameLine();
        if (custom::Button("Load Settings", ImVec2(150, 38))) {
          LoadConfig("config.cfg");
          SyncConfigToGlobals();
        }
      }
      custom::EndChild();
    }
  }
  ImGui::End();
}

// ============ DX11 GLOBALS ============
ID3D11Device *g_pd3dDevice = NULL;
ID3D11DeviceContext *g_pd3dDeviceContext = NULL;
IDXGISwapChain *g_pSwapChain = NULL;
ID3D11RenderTargetView *g_mainRenderTargetView = NULL;
static ID3D11BlendState *g_pBlendState = NULL;
HWND hwndOverlay = NULL;
int screenWidth = 1920, screenHeight = 1080;

ImFont *Inter_S = nullptr;
ImFont *Inter_S_1 = nullptr;
ImFont *Inter_S_2 = nullptr;
ImFont *Inter_S_3 = nullptr;
ImFont *Inter_B = nullptr;
ImFont *Icon = nullptr;
ImFont *Icon_Arrow = nullptr;

// ============ THREAD SYNC ============
std::atomic<bool> running{true};
std::atomic<int> telemetryFps{0};
std::atomic<int> telemetryTargets{0};
std::atomic<float> telemetryScanMs{0.07f};
std::atomic<uint64_t> g_soundWallMs[129]{};
std::atomic<uint64_t> g_hitmarkerTime{0};
std::atomic<int> g_hitmarkerDamage{0};

// ============ MATH & VECTOR STRUCTS ============
struct Vec2 {
  float x = 0, y = 0;
};
struct Vec3 {
  float x = 0, y = 0, z = 0;
  inline float DistTo(const Vec3 &o) const {
    float dx = x - o.x, dy = y - o.y, dz = z - o.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }
  inline float DistToSq(const Vec3 &o) const {
    float dx = x - o.x, dy = y - o.y, dz = z - o.z;
    return dx * dx + dy * dy + dz * dz;
  }
  inline Vec3 operator+(const Vec3 &o) const {
    return {x + o.x, y + o.y, z + o.z};
  }
  inline Vec3 operator-(const Vec3 &o) const {
    return {x - o.x, y - o.y, z - o.z};
  }
  inline Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
};
struct Matrix4x4 {
  float m[4][4];
};
struct BoneJointData {
  Vec3 pos;
  char pad[0x14];
};

struct SoundEmission {
  Vec3 origin{};
  uint64_t timestamp = 0;
  int team = 0;
  char name[64] = {0};
};
static SoundEmission g_soundEvents[129]{};

// ============ WEAPON LOOKUP ============
static const char *WeaponNameFromDefIdx(uint16_t idx) {
  switch (idx) {
  case 1:
    return "Deagle";
  case 2:
    return "Elites";
  case 3:
    return "FiveSeven";
  case 4:
    return "Glock";
  case 7:
    return "AK-47";
  case 8:
    return "AUG";
  case 9:
    return "AWP";
  case 10:
    return "FAMAS";
  case 11:
    return "G3SG1";
  case 13:
    return "Galil";
  case 14:
    return "M249";
  case 16:
    return "M4A1";
  case 17:
    return "MAC-10";
  case 19:
    return "P90";
  case 23:
    return "MP5-SD";
  case 24:
    return "UMP-45";
  case 25:
    return "XM1014";
  case 26:
    return "PP-Bizon";
  case 27:
    return "MAG-7";
  case 28:
    return "Negev";
  case 29:
    return "Sawed-Off";
  case 30:
    return "Tec-9";
  case 31:
    return "Zeus";
  case 32:
    return "P2000";
  case 33:
    return "MP7";
  case 34:
    return "MP9";
  case 35:
    return "Nova";
  case 36:
    return "P250";
  case 38:
    return "SCAR-20";
  case 39:
    return "SG553";
  case 40:
    return "SSG08";
  case 42:
    return "Knife";
  case 43:
    return "Flash";
  case 44:
    return "HE";
  case 45:
    return "Smoke";
  case 46:
    return "Molotov";
  case 47:
    return "Decoy";
  case 48:
    return "Incen";
  case 49:
    return "C4";
  case 60:
    return "M4A1-S";
  case 61:
    return "USP-S";
  case 63:
    return "CZ75";
  case 64:
    return "R8";
  case 500:
  case 503:
  case 505:
  case 506:
  case 507:
  case 508:
  case 509:
  case 512:
  case 514:
  case 515:
  case 516:
  case 519:
  case 520:
  case 522:
  case 523:
  case 525:
    return "Knife";
  default:
    return nullptr;
  }
}

static int WeaponIDFromDefIdx(uint16_t idx) {
  switch (idx) {
  case 61:
  case 32:
    return WEAPON_SILENCED_PISTOL;
  case 1:
  case 2:
  case 3:
  case 4:
  case 30:
  case 36:
  case 63:
  case 64:
    return WEAPON_PISTOL;
  case 17:
  case 23:
  case 33:
    return WEAPON_SMG1;
  case 34:
  case 19:
  case 26:
    return WEAPON_SMG2;
  case 24:
    return WEAPON_SMG3;
  case 7:
  case 8:
  case 10:
  case 13:
  case 16:
  case 60:
  case 39:
  case 14:
  case 28:
    return WEAPON_RIFLE;
  case 9:
  case 40:
    return WEAPON_SNIPER;
  case 11:
  case 38:
    return WEAPON_AUTOSNIPER;
  default:
    return -1;
  }
}

// ============ ENTITY STRUCTS (OPTIMIZED: bones[23], weaponName[24])
// ============
struct EntityData {
  bool valid = false;
  bool hasBomb = false;
  bool spotted = false;
  bool spottedByLocal = false;
  int health = 0;
  int armor = 0;
  int ammo = -1;
  int team = 0;
  int shotsFired = 0;
  int entitySlot = -1;
  uintptr_t pawn = 0;
  uintptr_t boneArrayAddress = 0;
  char name[64] = {0};
  char weaponName[24] = {0}; // OPT: was 32
  Vec3 origin{};
  Vec3 headPos{};
  Vec3 velocity{};
  Vec2 eyeAngles{};
  float lastSoundTime = 0.0f;
  bool hasBones = false;
  Vec3 bones[32]{};
  // Pre-transformed screen coordinates (cached per snapshot)
  Vec2 screenOrigin{};
  Vec2 screenHead{};
  Vec2 screenBones[32]{};
  bool screenOriginValid = false;
  bool screenHeadValid = false;
  bool screenBonesValid[32]{};
};

struct BombState {
  bool valid = false;
  bool ticking = false;
  bool defusing = false;
  bool defused = false;
  bool canDefuseInTime = true;
  int site = -1;
  float curTime = 0.f;
  float blowTime = 0.f;
  float defuseCountDown = 0.f;
  float timerLength = 40.f;
  float defuseLength = 10.f;
  float remainingBlow = 0.f;
  float remainingDefuse = 0.f;
  Vec3 worldPos{};
  std::chrono::steady_clock::time_point plantWallTime;
  std::chrono::steady_clock::time_point captureTime;
  bool wallTimeCaptured = false;
};

struct WorldItem {
  bool valid = false;
  bool isC4 = false;
  bool isProjectile = false;
  int projectileType = 0;
  char name[40] = {0};
  Vec3 position{};
  Vec3 velocity{};
};

struct ProjectileItem {
  bool valid = false;
  int projectileType = 0;
  char name[32] = {0};
  Vec3 position{};
  Vec3 velocity{};
  float spawnTime = 0.0f;
  int bounces = 0;
  bool exploded = false;
  int trailCount = 0;
  Vec3 trailPoints[64]{};
};

struct GrenadeLineup {
  int mapIdx;
  const char *mapName;
  const char *title;
  const char *nadeType;
  int throwType;
  Vec3 standPos;
  float targetPitch;
  float targetYaw;
};

static const GrenadeLineup g_lineups[] = {{0,
                                           "Mirage",
                                           "Window Smoke (Spawn 1)",
                                           "Smoke",
                                           1,
                                           {1296.00f, 32.00f, -103.97f},
                                           -28.97f,
                                           -165.84f},
                                          {0,
                                           "Mirage",
                                           "Window Smoke (Spawn 2)",
                                           "Smoke",
                                           1,
                                           {1216.00f, -16.00f, -101.94f},
                                           -44.09f,
                                           -166.32f},
                                          {0,
                                           "Mirage",
                                           "Window Smoke (Spawn 3)",
                                           "Smoke",
                                           1,
                                           {1216.00f, -115.00f, -101.94f},
                                           -46.02f,
                                           -168.74f},
                                          {0,
                                           "Mirage",
                                           "Window Smoke (Spawn 4)",
                                           "Smoke",
                                           1,
                                           {1216.00f, -211.00f, -100.62f},
                                           -45.99f,
                                           -170.31f},
                                          {0,
                                           "Mirage",
                                           "Window Smoke (Spawn 5)",
                                           "Smoke",
                                           1,
                                           {1296.00f, -352.00f, -104.13f},
                                           -42.26f,
                                           -173.42f},
                                          {0,
                                           "Mirage",
                                           "Window Smoke (Trash Can)",
                                           "Smoke",
                                           1,
                                           {1422.95f, 36.47f, -103.97f},
                                           -22.67f,
                                           -166.40f},
                                          {0,
                                           "Mirage",
                                           "A Stairs Smoke (T Pillar)",
                                           "Smoke",
                                           1,
                                           {1422.97f, -367.97f, -103.97f},
                                           -45.86f,
                                           -146.77f},
                                          {0,
                                           "Mirage",
                                           "A Jungle / Con (T Pillar)",
                                           "Smoke",
                                           0,
                                           {1422.97f, -367.97f, -103.97f},
                                           -41.07f,
                                           -150.90f},
                                          {0,
                                           "Mirage",
                                           "A CT / Ticket (T-Spawn)",
                                           "Smoke",
                                           1,
                                           {1422.97f, -248.03f, -103.97f},
                                           -51.31f,
                                           -136.39f},
                                          {0,
                                           "Mirage",
                                           "Connector Smoke (Spawn 1)",
                                           "Smoke",
                                           1,
                                           {1216.00f, -211.00f, -101.00f},
                                           -48.66f,
                                           -158.90f},
                                          {0,
                                           "Mirage",
                                           "Connector Smoke (Spawn 2)",
                                           "Smoke",
                                           1,
                                           {1296.00f, -352.00f, -104.13f},
                                           -42.83f,
                                           -173.23f},
                                          {0,
                                           "Mirage",
                                           "Connector Smoke (Trash Can)",
                                           "Smoke",
                                           1,
                                           {1422.97f, 70.97f, -48.63f},
                                           -6.37f,
                                           -156.80f},
                                          {0,
                                           "Mirage",
                                           "Palace Waterfall Smoke",
                                           "Smoke",
                                           0,
                                           {16.07f, -2360.03f, 23.87f},
                                           -17.69f,
                                           48.71f},
                                          {0,
                                           "Mirage",
                                           "B Apps Popflash (Bench)",
                                           "Flash",
                                           0,
                                           {-2635.97f, 104.03f, -95.46f},
                                           -20.50f,
                                           41.67f},
                                          {0,
                                           "Mirage",
                                           "B Apps Popflash (Market)",
                                           "Flash",
                                           0,
                                           {-1999.99f, -246.97f, -95.08f},
                                           -19.04f,
                                           75.48f},
                                          {0,
                                           "Mirage",
                                           "B Short / Cat Smoke",
                                           "Smoke",
                                           0,
                                           {110.00f, 480.00f, -95.00f},
                                           -32.50f,
                                           -172.00f},
                                          {0,
                                           "Mirage",
                                           "B Market Door Smoke",
                                           "Smoke",
                                           1,
                                           {-80.00f, 550.00f, -95.00f},
                                           -38.00f,
                                           -155.00f},
                                          {1,
                                           "Inferno",
                                           "B Coffins Smoke (Half Wall)",
                                           "Smoke",
                                           1,
                                           {423.74f, 1761.97f, 198.03f},
                                           -11.82f,
                                           83.87f},
                                          {1,
                                           "Inferno",
                                           "B Coffins Smoke (Car)",
                                           "Smoke",
                                           0,
                                           {479.05f, 2016.97f, 200.00f},
                                           -43.43f,
                                           86.29f},
                                          {1,
                                           "Inferno",
                                           "B CT Smoke (Banana Wall)",
                                           "Smoke",
                                           0,
                                           {290.33f, 1564.97f, 185.75f},
                                           -29.60f,
                                           63.35f},
                                          {1,
                                           "Inferno",
                                           "B CT Smoke (Half Wall)",
                                           "Smoke",
                                           1,
                                           {423.75f, 1761.96f, 197.87f},
                                           -13.17f,
                                           63.48f},
                                          {1,
                                           "Inferno",
                                           "A Arch Smoke (Alt Mid)",
                                           "Smoke",
                                           1,
                                           {617.06f, -264.97f, 152.87f},
                                           -52.46f,
                                           51.06f},
                                          {1,
                                           "Inferno",
                                           "A Library Smoke (Alt Mid)",
                                           "Smoke",
                                           1,
                                           {616.24f, -264.97f, 152.87f},
                                           -39.13f,
                                           37.15f},
                                          {1,
                                           "Inferno",
                                           "A Pit Molotov (Alt Mid)",
                                           "Molotov",
                                           1,
                                           {540.00f, 120.00f, 140.00f},
                                           -35.00f,
                                           45.00f},
                                          {1,
                                           "Inferno",
                                           "Banana Entry Popflash",
                                           "Flash",
                                           0,
                                           {120.00f, 1350.00f, 160.00f},
                                           -48.00f,
                                           85.00f},
                                          {1,
                                           "Inferno",
                                           "B New Box Molotov",
                                           "Molotov",
                                           0,
                                           {420.00f, 1800.00f, 198.00f},
                                           -28.50f,
                                           80.00f},
                                          {1,
                                           "Inferno",
                                           "A Long / Quad Smoke",
                                           "Smoke",
                                           1,
                                           {480.00f, -400.00f, 150.00f},
                                           -46.00f,
                                           30.00f},
                                          {2,
                                           "Dust2",
                                           "Xbox Smoke (T-Spawn Fast)",
                                           "Smoke",
                                           1,
                                           {-299.97f, -1163.76f, 136.98f},
                                           -12.23f,
                                           90.17f},
                                          {2,
                                           "Dust2",
                                           "Xbox Smoke (Spawn 1)",
                                           "Smoke",
                                           1,
                                           {-240.00f, -1696.00f, 66.03f},
                                           -10.11f,
                                           85.62f},
                                          {2,
                                           "Dust2",
                                           "Xbox Smoke (Spawn 2)",
                                           "Smoke",
                                           1,
                                           {-352.00f, -1696.00f, 66.03f},
                                           -10.66f,
                                           83.02f},
                                          {2,
                                           "Dust2",
                                           "Xbox Smoke (Spawn 3)",
                                           "Smoke",
                                           1,
                                           {-416.00f, -1608.00f, 59.56f},
                                           -24.29f,
                                           82.57f},
                                          {2,
                                           "Dust2",
                                           "Xbox Smoke (Spawn 4)",
                                           "Smoke",
                                           1,
                                           {-304.00f, -1608.00f, 52.03f},
                                           -8.83f,
                                           84.56f},
                                          {2,
                                           "Dust2",
                                           "A Cross Smoke (Top Mid)",
                                           "Smoke",
                                           1,
                                           {148.03f, 182.85f, 71.87f},
                                           -43.75f,
                                           77.87f},
                                          {2,
                                           "Dust2",
                                           "A Long Smoke (from Boost)",
                                           "Smoke",
                                           0,
                                           {654.29f, 2314.97f, 23.22f},
                                           -30.30f,
                                           -39.56f},
                                          {2,
                                           "Dust2",
                                           "A Long Smoke (from Plat)",
                                           "Smoke",
                                           1,
                                           {828.69f, 2751.89f, 160.27f},
                                           4.94f,
                                           -58.28f},
                                          {2,
                                           "Dust2",
                                           "B Doors Smoke (T Plat)",
                                           "Smoke",
                                           0,
                                           {-1667.97f, -256.03f, 191.90f},
                                           -14.59f,
                                           82.20f},
                                          {2,
                                           "Dust2",
                                           "B Doors Smoke (Fast T-Plat)",
                                           "Smoke",
                                           0,
                                           {-1450.39f, -551.51f, 191.50f},
                                           -10.64f,
                                           87.03f},
                                          {2,
                                           "Dust2",
                                           "B Doors Smoke (Upper Tunnel)",
                                           "Smoke",
                                           0,
                                           {-1837.97f, 982.03f, 103.78f},
                                           -61.38f,
                                           67.82f},
                                          {2,
                                           "Dust2",
                                           "B Window Smoke (Catwalk)",
                                           "Smoke",
                                           0,
                                           {-149.03f, 1220.75f, 63.48f},
                                           -5.97f,
                                           127.25f},
                                          {2,
                                           "Dust2",
                                           "B Jump Box Flash (Tunnels)",
                                           "Flash",
                                           0,
                                           {-1682.03f, 1272.90f, 98.19f},
                                           -61.42f,
                                           107.89f},
                                          {2,
                                           "Dust2",
                                           "Long Doors Flash (Outside)",
                                           "Flash",
                                           0,
                                           {1050.00f, 400.00f, 15.00f},
                                           -45.00f,
                                           40.00f},
                                          {3,
                                           "Nuke",
                                           "Outside Left Wall (T Roof)",
                                           "Smoke",
                                           0,
                                           {-1832.00f, -1160.00f, -352.00f},
                                           -24.83f,
                                           -14.60f},
                                          {3,
                                           "Nuke",
                                           "A Heaven Smoke (T Roof)",
                                           "Smoke",
                                           0,
                                           {-1832.00f, -1160.00f, -351.97f},
                                           -40.46f,
                                           5.75f},
                                          {3,
                                           "Nuke",
                                           "Outside Far Cross (Outside)",
                                           "Smoke",
                                           0,
                                           {-311.79f, -1919.83f, -401.91f},
                                           -13.07f,
                                           -4.13f},
                                          {3,
                                           "Nuke",
                                           "A Heaven Smoke (Alt Main)",
                                           "Smoke",
                                           0,
                                           {-16.18f, -1466.68f, 16.12f},
                                           -21.81f,
                                           46.49f},
                                          {3,
                                           "Nuke",
                                           "Ramp Entry Flash (T Lobby)",
                                           "Flash",
                                           0,
                                           {-800.00f, -1200.00f, -400.00f},
                                           -42.00f,
                                           -45.00f},
                                          {3,
                                           "Nuke",
                                           "A Site Hut Molotov (Roof)",
                                           "Molotov",
                                           0,
                                           {-600.00f, -800.00f, -200.00f},
                                           -28.00f,
                                           10.00f},
                                          {4,
                                           "Anubis",
                                           "B Connector Smoke",
                                           "Smoke",
                                           0,
                                           {-1350.0f, -600.0f, 80.0f},
                                           -34.0f,
                                           68.0f},
                                          {4,
                                           "Anubis",
                                           "B Heaven Smoke (T Approach)",
                                           "Smoke",
                                           1,
                                           {-1400.0f, -700.0f, 80.0f},
                                           -41.0f,
                                           75.0f},
                                          {4,
                                           "Anubis",
                                           "A Main Heaven Smoke",
                                           "Smoke",
                                           0,
                                           {850.0f, 600.0f, 100.0f},
                                           -36.0f,
                                           -80.0f},
                                          {4,
                                           "Anubis",
                                           "A Plate Molotov (A Main)",
                                           "Molotov",
                                           0,
                                           {700.0f, 750.0f, 100.0f},
                                           -28.0f,
                                           -65.0f},
                                          {4,
                                           "Anubis",
                                           "Mid Canal Smoke (Bridge)",
                                           "Smoke",
                                           1,
                                           {-150.0f, 300.0f, 20.0f},
                                           -32.0f,
                                           15.0f},
                                          {4,
                                           "Anubis",
                                           "B Main Entry Flash",
                                           "Flash",
                                           0,
                                           {-1200.0f, -400.0f, 80.0f},
                                           -52.0f,
                                           85.0f},
                                          {5,
                                           "Ancient",
                                           "B Long Ramp Smoke (T Spawn)",
                                           "Smoke",
                                           0,
                                           {-1400.0f, -1200.0f, 120.0f},
                                           -36.0f,
                                           55.0f},
                                          {5,
                                           "Ancient",
                                           "B Cave / Short Smoke",
                                           "Smoke",
                                           1,
                                           {-1250.0f, -1100.0f, 120.0f},
                                           -39.0f,
                                           70.0f},
                                          {5,
                                           "Ancient",
                                           "A Donut Smoke (T Mid)",
                                           "Smoke",
                                           0,
                                           {400.0f, -800.0f, 140.0f},
                                           -33.0f,
                                           -75.0f},
                                          {5,
                                           "Ancient",
                                           "A Temple Smoke (T Yard)",
                                           "Smoke",
                                           1,
                                           {550.0f, -750.0f, 140.0f},
                                           -42.0f,
                                           -85.0f},
                                          {5,
                                           "Ancient",
                                           "Mid Red Room Smoke",
                                           "Smoke",
                                           0,
                                           {-50.0f, -400.0f, 80.0f},
                                           -31.0f,
                                           20.0f},
                                          {5,
                                           "Ancient",
                                           "A Site Entry Flash",
                                           "Flash",
                                           0,
                                           {500.0f, -600.0f, 140.0f},
                                           -48.0f,
                                           -60.0f},
                                          {6,
                                           "Vertigo",
                                           "A Ramp Left Cross Smoke",
                                           "Smoke",
                                           0,
                                           {-1200.0f, -600.0f, 100.0f},
                                           -32.0f,
                                           40.0f},
                                          {6,
                                           "Vertigo",
                                           "A Ramp Right Cross Smoke",
                                           "Smoke",
                                           0,
                                           {-1150.0f, -700.0f, 100.0f},
                                           -35.0f,
                                           30.0f},
                                          {6,
                                           "Vertigo",
                                           "B Stairs Smoke (T Walk)",
                                           "Smoke",
                                           1,
                                           {-300.0f, 1200.0f, 120.0f},
                                           -40.0f,
                                           110.0f},
                                          {6,
                                           "Vertigo",
                                           "Mid Elevator Smoke",
                                           "Smoke",
                                           0,
                                           {-500.0f, 100.0f, 80.0f},
                                           -28.0f,
                                           10.0f},
                                          {6,
                                           "Vertigo",
                                           "A Sandbags Molotov",
                                           "Molotov",
                                           0,
                                           {-1000.0f, -500.0f, 100.0f},
                                           -25.0f,
                                           45.0f},
                                          {7,
                                           "Overpass",
                                           "B Monster Smoke (Water)",
                                           "Smoke",
                                           0,
                                           {-1500.0f, 300.0f, 10.0f},
                                           -33.0f,
                                           85.0f},
                                          {7,
                                           "Overpass",
                                           "B Short Bridge Smoke",
                                           "Smoke",
                                           1,
                                           {-1300.0f, 500.0f, 10.0f},
                                           -42.0f,
                                           95.0f},
                                          {7,
                                           "Overpass",
                                           "A Long / Toilets Smoke",
                                           "Smoke",
                                           0,
                                           {450.0f, -1100.0f, 180.0f},
                                           -36.0f,
                                           -80.0f},
                                          {7,
                                           "Overpass",
                                           "A Bank Smoke (Toilets)",
                                           "Smoke",
                                           1,
                                           {350.0f, -950.0f, 180.0f},
                                           -44.0f,
                                           -90.0f},
                                          {7,
                                           "Overpass",
                                           "B Barrels Molotov (Canal)",
                                           "Molotov",
                                           0,
                                           {-1100.0f, 650.0f, 10.0f},
                                           -26.0f,
                                           75.0f}};
static const int g_lineupCount = sizeof(g_lineups) / sizeof(g_lineups[0]);

struct SpectatorEntry {
  char name[64] = {0};
};

struct FrameSnapshot {
  std::chrono::high_resolution_clock::time_point timestamp{};
  Matrix4x4 viewMatrix{};
  Vec3 localOrigin{};
  int localTeam = 0;
  int localShotsFired = 0;
  uint32_t localFlags = 0;
  Vec3 localPunchAngle{};
  uint16_t localWeaponDefIdx = 0;
  char localWeaponName[24] = {0};
  Vec2 localViewAngles{};
  Vec3 localVelocity{};
  Vec3 localViewOffset{};
  float localNadeThrowStrength = 1.0f;
  bool localNadePinPulled = false;
  int localPlayerIndex = 0;
  uint64_t localSpottedMask = 0;
  uintptr_t localPawnAddr = 0;
  uintptr_t localCtrlAddr = 0;
  uint32_t localPawnHandle = 0;
  uintptr_t entityListAddr = 0;
  int count = 0;
  EntityData entities[128]{};
};

struct SlowSnapshot {
  BombState bomb{};
  WorldItem items[64]{};
  int itemCount = 0;
  ProjectileItem projectiles[16]{};
  int projectileCount = 0;
  SpectatorEntry spectators[8]{};
  int spectatorCount = 0;
  char currentMapName[32] = {0};
  int detectedMapIdx = 0;
};

// ============ FRAME CONTEXT (NEW: unified matrix + cached weapon config)
// ============ Single live matrix shared by ESP + aimbot + triggerbot.
// Eliminates 1-frame skew.
struct FrameContext {
  const FrameSnapshot *snap;
  const Matrix4x4 *viewMatrix;
  const WeaponConfig *wcfg; // active weapon config, resolved once per frame
  Vec3 localOrigin;
  int localTeam;
  int screenW, screenH;
  HWND foregroundHwnd;   // cached, refreshed on a 500ms timer
  DWORD foregroundPid;
};

static FrameSnapshot g_snapBuf[3];
static std::atomic<int> g_snapWriteIdx{0};
static std::atomic<int> g_snapInFlight{1};
static std::atomic<int> g_snapRenderReading{-1};
CRITICAL_SECTION g_snapLock;

struct SlowSnapshotSlot {
  SlowSnapshot data[3];
  std::atomic<int> writeIdx{0};
  std::atomic<int> publishedIdx{1};
};
static SlowSnapshotSlot g_slowRing;

// ============ ENTITY CACHING ============
struct EntityStatic {
  char name[64] = {0};
  int team = 0;
  char weaponName[24] = {0};
  uint32_t pawnHandle = 0;
  uintptr_t pawn = 0;
  uintptr_t controller = 0;
  uintptr_t gameSceneNode = 0;
  uintptr_t boneArrayAddress = 0;
  uint64_t lastNameRefresh = 0;
  uint32_t cachedWpnHandle = 0;
  float cachedSoundTime = 0.0f;
  bool hasBomb = false;
  bool active = false;
};
using CachedTarget = EntityStatic;

// ============ BATCHED READ BLOCKS ============
#pragma pack(push, 1)
struct PawnBatchLow {
  uintptr_t gameSceneNode;
  char pad_338_34C[0x14];
  int32_t health;
  char pad_350_3E7[0x97];
  uint8_t team;
  char pad_3E8_3F8[0x10];
  Vec3 velocity;
  char pad_404_410[0x0C];
};
static_assert(sizeof(PawnBatchLow) == 0xE0, "PawnBatchLow size mismatch");
static_assert(offsetof(PawnBatchLow, gameSceneNode) == (0x330 - 0x330), "gsn");
static_assert(offsetof(PawnBatchLow, health)        == (0x34C - 0x330), "hp");
static_assert(offsetof(PawnBatchLow, team)          == (0x3E7 - 0x330), "team");
static_assert(offsetof(PawnBatchLow, velocity)      == (0x3F8 - 0x330), "vel");

struct PawnOrigin {
  Vec3 origin;
};
static_assert(sizeof(PawnOrigin) == 0x0C, "PawnOrigin size mismatch");

struct PawnSpot {
  char pad_00_08[8];
  bool spotted;
  char pad_09_0C[3];
  uint32_t spottedByMask[2];
  char pad_14_20[12];
  float emitSoundTime;
  char pad_24_2C[8];
  int32_t shotsFired;
  char pad_30_44[20];
  int32_t armor;
};
static_assert(sizeof(PawnSpot) == 0x48, "PawnSpot size mismatch");
#pragma pack(pop)

// ============ SIMD + SCALAR OPTIMIZED W2S ============
struct PrecomputedW2S {
  __m128 r0_x, r0_y, r0_z, r0_w;
  __m128 r1_x, r1_y, r1_z, r1_w;
  __m128 r3_x, r3_y, r3_z, r3_w;
  float f_r0_x = 0.0f, f_r0_y = 0.0f, f_r0_z = 0.0f, f_r0_w = 0.0f;
  float f_r1_x = 0.0f, f_r1_y = 0.0f, f_r1_z = 0.0f, f_r1_w = 0.0f;
  float f_r3_x = 0.0f, f_r3_y = 0.0f, f_r3_z = 0.0f, f_r3_w = 0.0f;
  float halfW = 0.0f, halfH = 0.0f;
};
static PrecomputedW2S g_w2sCache;

inline void UpdateW2SCache(const Matrix4x4 &vm, float sw, float sh,
                           PrecomputedW2S &c) {
  c.halfW = sw * 0.5f;
  c.halfH = sh * 0.5f;
  float hw = c.halfW;
  float nh = -c.halfH;

  c.f_r0_x = vm.m[0][0] * hw;
  c.f_r0_y = vm.m[0][1] * hw;
  c.f_r0_z = vm.m[0][2] * hw;
  c.f_r0_w = vm.m[0][3] * hw;

  c.f_r1_x = vm.m[1][0] * nh;
  c.f_r1_y = vm.m[1][1] * nh;
  c.f_r1_z = vm.m[1][2] * nh;
  c.f_r1_w = vm.m[1][3] * nh;

  c.f_r3_x = vm.m[3][0];
  c.f_r3_y = vm.m[3][1];
  c.f_r3_z = vm.m[3][2];
  c.f_r3_w = vm.m[3][3];

  c.r0_x = _mm_set1_ps(c.f_r0_x);
  c.r0_y = _mm_set1_ps(c.f_r0_y);
  c.r0_z = _mm_set1_ps(c.f_r0_z);
  c.r0_w = _mm_set1_ps(c.f_r0_w);

  c.r1_x = _mm_set1_ps(c.f_r1_x);
  c.r1_y = _mm_set1_ps(c.f_r1_y);
  c.r1_z = _mm_set1_ps(c.f_r1_z);
  c.r1_w = _mm_set1_ps(c.f_r1_w);

  c.r3_x = _mm_set1_ps(c.f_r3_x);
  c.r3_y = _mm_set1_ps(c.f_r3_y);
  c.r3_z = _mm_set1_ps(c.f_r3_z);
  c.r3_w = _mm_set1_ps(c.f_r3_w);
}

inline bool WorldToScreen(const Vec3 &world, const PrecomputedW2S &c,
                          Vec2 &screen) {
  float w = c.f_r3_x * world.x + c.f_r3_y * world.y + c.f_r3_z * world.z + c.f_r3_w;
  if (w < 0.001f)
    return false;
  float invW = 1.0f / w;
  float x = c.f_r0_x * world.x + c.f_r0_y * world.y + c.f_r0_z * world.z + c.f_r0_w;
  float y = c.f_r1_x * world.x + c.f_r1_y * world.y + c.f_r1_z * world.z + c.f_r1_w;
  screen.x = c.halfW + x * invW;
  screen.y = c.halfH + y * invW;
  return true;
}

inline bool WorldToScreen(const Vec3 &world, const Matrix4x4 &vm,
                          Vec2 &screen) {
  return WorldToScreen(world, g_w2sCache, screen);
}

inline void BatchW2S_4Way(const Vec3 in[4], const PrecomputedW2S &c,
                          Vec2 out[4], bool valid[4]) {
  const __m128 X = _mm_setr_ps(in[0].x, in[1].x, in[2].x, in[3].x);
  const __m128 Y = _mm_setr_ps(in[0].y, in[1].y, in[2].y, in[3].y);
  const __m128 Z = _mm_setr_ps(in[0].z, in[1].z, in[2].z, in[3].z);

  const __m128 w = _mm_add_ps(
      _mm_add_ps(_mm_mul_ps(X, c.r3_x), _mm_mul_ps(Y, c.r3_y)),
      _mm_add_ps(_mm_mul_ps(Z, c.r3_z), c.r3_w));

  const __m128 invW = _mm_div_ps(_mm_set1_ps(1.0f), w);

  const __m128 px = _mm_add_ps(
      _mm_mul_ps(_mm_add_ps(
                     _mm_add_ps(_mm_mul_ps(X, c.r0_x), _mm_mul_ps(Y, c.r0_y)),
                     _mm_add_ps(_mm_mul_ps(Z, c.r0_z), c.r0_w)),
                 invW),
      _mm_set1_ps(c.halfW));

  const __m128 py = _mm_add_ps(
      _mm_mul_ps(_mm_add_ps(
                     _mm_add_ps(_mm_mul_ps(X, c.r1_x), _mm_mul_ps(Y, c.r1_y)),
                     _mm_add_ps(_mm_mul_ps(Z, c.r1_z), c.r1_w)),
                 invW),
      _mm_set1_ps(c.halfH));

  alignas(16) float rx[4], ry[4], rw[4];
  _mm_store_ps(rx, px);
  _mm_store_ps(ry, py);
  _mm_store_ps(rw, w);

  for (int i = 0; i < 4; i++) {
    valid[i] = rw[i] >= 0.001f;
    out[i].x = rx[i];
    out[i].y = ry[i];
  }
}

inline void ProjectBonesChunked(const Vec3 bones[32], const uint8_t *indices,
                                const PrecomputedW2S &cache,
                                Vec2 out[32], bool ok[32], size_t count) {
  size_t i = 0;
  for (; i + 4 <= count; i += 4) {
    Vec3 in[4] = {bones[indices[i]], bones[indices[i+1]],
                  bones[indices[i+2]], bones[indices[i+3]]};
    Vec2 o[4]; bool v[4];
    BatchW2S_4Way(in, cache, o, v);
    for (int k = 0; k < 4; k++) {
      const Vec3 &b = in[k];
      out[indices[i+k]] = o[k];
      ok[indices[i+k]] = v[k] && (b.x || b.y || b.z);
    }
  }
  for (; i < count; i++) {
    uint8_t bi = indices[i];
    const Vec3 &b = bones[bi];
    if (b.x || b.y || b.z) {
      ok[bi] = WorldToScreen(b, cache, out[bi]);
    } else {
      ok[bi] = false;
    }
  }
}

// ============ DX11 SRV HELPER ============
ID3D11ShaderResourceView *CreateSRVFromMemoryDX11(ID3D11Device *dev,
                                                  const unsigned char *data,
                                                  size_t sz) {
  if (!dev || !data || sz == 0)
    return nullptr;
  int w = 0, h = 0, comp = 0;
  unsigned char *img = stbi_load_from_memory(data, (int)sz, &w, &h, &comp, 4);
  if (!img)
    return nullptr;
  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = w;
  desc.Height = h;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  D3D11_SUBRESOURCE_DATA init = {};
  init.pSysMem = img;
  init.SysMemPitch = w * 4;
  ID3D11Texture2D *tex = nullptr;
  HRESULT hr = dev->CreateTexture2D(&desc, &init, &tex);
  stbi_image_free(img);
  if (FAILED(hr) || !tex)
    return nullptr;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = desc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  ID3D11ShaderResourceView *srv = nullptr;
  hr = dev->CreateShaderResourceView(tex, &srvDesc, &srv);
  tex->Release();
  return srv;
}
static int s_prevHealth[65] = {0};
static uint64_t s_lastSpottedMs[65] = {0};

// ============ MEMORY WORKER THREAD ============
void MemoryWorkerThread() {
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
  CachedTarget targets[129]{};
  static int s_localPlayerIndex = 1;
  auto lastCacheTime =
      std::chrono::steady_clock::now() - std::chrono::seconds(10);

  while (running) {
    auto tStart = std::chrono::high_resolution_clock::now();

    if (!hGame || !clientBase) {
      HWND hwndCS2 = FindWindowA("SDL_app", "Counter-Strike 2");
      if (!hwndCS2)
        hwndCS2 = FindWindowA(NULL, "Counter-Strike 2");
      if (hwndCS2)
        GetWindowThreadProcessId(hwndCS2, &pid);
      if (!pid) {
        UniqueHandle hSnapP(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
        if (hSnapP) {
          PROCESSENTRY32W pe = {sizeof(pe)};
          if (Process32FirstW(hSnapP.get(), &pe)) {
            do {
              if (_wcsicmp(pe.szExeFile, L"cs2.exe") == 0) {
                pid = pe.th32ProcessID;
                break;
              }
            } while (Process32NextW(hSnapP.get(), &pe));
          }
        }
      }
      if (pid) {
        hGame = OpenProcess(
            PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (hGame) {
          UniqueHandle hSnap(CreateToolhelp32Snapshot(
              TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid));
          if (hSnap) {
            MODULEENTRY32W me = {sizeof(me)};
            if (Module32FirstW(hSnap.get(), &me)) {
              do {
                wchar_t obfw_cdll[12] = {};
                OBF_WDEC(obfw_cdll, L"client.dll");
                if (wcsicmp(me.szModule, obfw_cdll) == 0) {
                  clientBase = reinterpret_cast<uintptr_t>(me.modBaseAddr);
                  break;
                }
              } while (Module32NextW(hSnap.get(), &me));
            }
          }
        }
      }
      if (!hGame || !clientBase) {
        static auto s_lastWait =
            std::chrono::steady_clock::now() - std::chrono::seconds(10);
        auto nowWait = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(nowWait -
                                                             s_lastWait)
                .count() >= 4) {
          ConsoleLogger::Log(ConsoleLogger::LOG_WARN,
                             "Waiting for Counter-Strike 2 (cs2.exe)...");
          s_lastWait = nowWait;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        continue;
      } else {
        ConsoleLogger::Log(ConsoleLogger::LOG_SUCCESS,
                           "Attached to Counter-Strike 2 (PID: %lu)", pid);
        std::fill(std::begin(s_prevHealth), std::end(s_prevHealth), 0);
        std::fill(std::begin(s_lastSpottedMs), std::end(s_lastSpottedMs), 0);
        ConsoleLogger::Log(ConsoleLogger::LOG_SUCCESS,
                           "Resolved client.dll base: 0x%012llX",
                           (unsigned long long)clientBase);
        ConsoleLogger::PrintOffsetTable(clientBase);
        ConsoleLogger::Log(ConsoleLogger::LOG_INFO,
                           "Memory stream active. Reading entity pipeline...");
      }
    }

    FrameSnapshot snap{};
    // OPT: view matrix read removed — render loop re-reads fresh anyway
    ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwViewAngles),
                      &snap.localViewAngles, sizeof(snap.localViewAngles),
                      nullptr);

    uintptr_t entityList = 0;
    if (!ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList),
                           &entityList, sizeof(entityList), nullptr)) {
      static auto s_lastEntErr =
          std::chrono::steady_clock::now() - std::chrono::seconds(5);
      auto nowErr = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::seconds>(nowErr -
                                                           s_lastEntErr)
              .count() >= 4) {
        ConsoleLogger::Log(
            ConsoleLogger::LOG_ERROR,
            "RPM failed on dwEntityList (0x%012llX) - Error: %lu",
            (unsigned long long)(clientBase + O::dwEntityList), GetLastError());
        s_lastEntErr = nowErr;
      }
    }

    uintptr_t localPawn = 0, localCtrl = 0;
    uint32_t lpPawnH = 0;
    ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwLocalPlayerController),
                      &localCtrl, sizeof(localCtrl), nullptr);
    if (localCtrl && entityList) {
      ReadProcessMemory(hGame, (LPCVOID)(localCtrl + O::m_hPlayerPawn),
                        &lpPawnH, sizeof(lpPawnH), nullptr);
      if (lpPawnH && lpPawnH != 0xFFFFFFFF) {
        uintptr_t pListEntry = 0;
        ReadProcessMemory(
            hGame, (LPCVOID)(entityList + 0x10 + 8 * ((lpPawnH & 0x7FFF) >> 9)),
            &pListEntry, sizeof(pListEntry), nullptr);
        if (pListEntry)
          ReadProcessMemory(hGame,
                            (LPCVOID)(pListEntry + 0x78 * (lpPawnH & 0x1FF)),
                            &localPawn, sizeof(localPawn), nullptr);
      }
    }
    if (!localPawn)
      ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwLocalPlayerPawn),
                        &localPawn, sizeof(localPawn), nullptr);

    snap.entityListAddr = entityList;
    snap.localCtrlAddr = localCtrl;
    snap.localPawnAddr = localPawn;
    snap.localPawnHandle = lpPawnH;

    if (localPawn) {
      constexpr uintptr_t kBase = 0x340;
      constexpr uintptr_t kEnd = 0x2000;
      BYTE localBuf[kEnd - kBase]{};
      if (ReadProcessMemory(hGame, (LPCVOID)(localPawn + kBase), localBuf,
                            sizeof(localBuf), nullptr)) {
        uint8_t localTeamRaw = localBuf[O::m_iTeamNum - kBase];
        snap.localTeam = localTeamRaw;
        memcpy(&snap.localOrigin, localBuf + (O::m_vOldOrigin - kBase),
               sizeof(snap.localOrigin));
        memcpy(&snap.localShotsFired, localBuf + (O::m_iShotsFired - kBase),
               sizeof(snap.localShotsFired));
        snap.localFlags =
            *reinterpret_cast<uint32_t *>(localBuf + (O::m_fFlags - kBase));
        snap.localPunchAngle = {0.0f, 0.0f, 0.0f};
        Vec3 directPunch{};
        memcpy(&directPunch, localBuf + (O::m_aimPunchAngleDirect - kBase), sizeof(directPunch));
        if (directPunch.x != 0.0f || directPunch.y != 0.0f) {
          snap.localPunchAngle = directPunch;
        } else if (snap.localShotsFired > 0) {
          uintptr_t punchSvc = *reinterpret_cast<uintptr_t *>(localBuf + (O::m_pAimPunchServices - kBase));
          if (punchSvc) {
            Vec3 pAngle{};
            if (ReadProcessMemory(hGame, (LPCVOID)(punchSvc + O::m_predictableBaseAngle), &pAngle, sizeof(pAngle), nullptr)) {
              if (pAngle.x != 0.0f || pAngle.y != 0.0f)
                snap.localPunchAngle = pAngle;
            }
          }
          if (snap.localPunchAngle.x == 0.0f && snap.localPunchAngle.y == 0.0f) {
            uintptr_t camSvc = *reinterpret_cast<uintptr_t *>(localBuf + (O::m_pCameraServices - kBase));
            if (camSvc) {
              Vec3 viewPunch{};
              if (ReadProcessMemory(hGame, (LPCVOID)(camSvc + O::m_vecCsViewPunchAngle), &viewPunch, sizeof(viewPunch), nullptr)) {
                if (viewPunch.x != 0.0f || viewPunch.y != 0.0f)
                  snap.localPunchAngle = viewPunch;
              }
            }
          }
        }
        const uint32_t *lpMask = reinterpret_cast<const uint32_t *>(
            localBuf + (O::m_entitySpottedState + O::m_bSpottedByMask - kBase));
        snap.localSpottedMask =
            (uint64_t)lpMask[0] | ((uint64_t)lpMask[1] << 32);
        ReadProcessMemory(hGame, (LPCVOID)(localPawn + O::m_vecViewOffset),
                          &snap.localViewOffset, sizeof(snap.localViewOffset),
                          nullptr);
        ReadProcessMemory(hGame, (LPCVOID)(localPawn + O::m_absVelocity),
                          &snap.localVelocity, sizeof(snap.localVelocity),
                          nullptr);
      }

      static auto s_lastSensRead =
          std::chrono::steady_clock::now() - std::chrono::seconds(10);
      auto nowSens = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(nowSens -
                                                                s_lastSensRead)
              .count() >= 500) {
        uintptr_t sensPtr = 0;
        if (ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwSensitivity),
                              &sensPtr, sizeof(sensPtr), nullptr) &&
            sensPtr) {
          float liveSens = 0.0f;
          if (ReadProcessMemory(
                  hGame, (LPCVOID)(sensPtr + O::dwSensitivity_sensitivity),
                  &liveSens, sizeof(liveSens), nullptr)) {
            if (liveSens >= 0.05f && liveSens <= 25.0f)
              g_gameSens = liveSens;
          }
        }
        s_lastSensRead = nowSens;
      }
    }

    if (entityList) {
      if (localPawn) {
        uintptr_t localWpnSvc = 0;
        ReadProcessMemory(hGame, (LPCVOID)(localPawn + O::m_pWeaponSvc),
                          &localWpnSvc, sizeof(localWpnSvc), nullptr);
        if (localWpnSvc) {
          uint32_t activeWpnH = 0;
          ReadProcessMemory(hGame, (LPCVOID)(localWpnSvc + O::m_hActiveWpn),
                            &activeWpnH, sizeof(activeWpnH), nullptr);
          if (activeWpnH && activeWpnH != 0xFFFFFFFF) {
            uintptr_t wpnListEntry = 0;
            ReadProcessMemory(
                hGame,
                (LPCVOID)(entityList + 0x10 + 8 * ((activeWpnH & 0x7FFF) >> 9)),
                &wpnListEntry, sizeof(wpnListEntry), nullptr);
            if (wpnListEntry) {
              uintptr_t wpnEnt = 0;
              ReadProcessMemory(
                  hGame, (LPCVOID)(wpnListEntry + 0x78 * (activeWpnH & 0x1FF)),
                  &wpnEnt, sizeof(wpnEnt), nullptr);
              if (!wpnEnt)
                ReadProcessMemory(
                    hGame,
                    (LPCVOID)(wpnListEntry + 0x70 * (activeWpnH & 0x1FF)),
                    &wpnEnt, sizeof(wpnEnt), nullptr);
              if (!wpnEnt)
                ReadProcessMemory(hGame,
                                  (LPCVOID)(localPawn + O::m_pClippingWeapon),
                                  &wpnEnt, sizeof(wpnEnt), nullptr);
              if (wpnEnt) {
                uint16_t defIdx = 0;
                // OPT: consistent offset 0x11A8+0x50+0x1BA = 0x13B2
                ReadProcessMemory(hGame,
                                  (LPCVOID)(wpnEnt + O::m_AttrMgr +
                                            O::m_Item_off + O::m_iItemDefIdx),
                                  &defIdx, sizeof(defIdx), nullptr);
                if (!defIdx) {
                  uintptr_t ident = 0;
                  ReadProcessMemory(hGame, (LPCVOID)(wpnEnt + O::m_pEntity_id),
                                    &ident, sizeof(ident), nullptr);
                  if (ident) {
                    uintptr_t namePtr = 0;
                    ReadProcessMemory(hGame,
                                      (LPCVOID)(ident + O::m_designerName),
                                      &namePtr, sizeof(namePtr), nullptr);
                    if (namePtr) {
                      char wDn[32] = {};
                      ReadProcessMemory(hGame, (LPCVOID)namePtr, wDn,
                                        sizeof(wDn) - 1, nullptr);
                      if (strstr(wDn, "flashbang"))
                        defIdx = 43;
                      else if (strstr(wDn, "hegrenade"))
                        defIdx = 44;
                      else if (strstr(wDn, "smokegrenade"))
                        defIdx = 45;
                      else if (strstr(wDn, "molotov"))
                        defIdx = 46;
                      else if (strstr(wDn, "decoy"))
                        defIdx = 47;
                      else if (strstr(wDn, "incgrenade"))
                        defIdx = 48;
                    }
                  }
                }
                snap.localWeaponDefIdx = defIdx;
                const char *wn =
                    defIdx ? WeaponNameFromDefIdx(defIdx) : nullptr;
                if (wn)
                  strncpy(snap.localWeaponName, wn,
                          sizeof(snap.localWeaponName) - 1);
                if (defIdx >= 43 && defIdx <= 48) {
                  bool pinPulled = false;
                  ReadProcessMemory(hGame, (LPCVOID)(wpnEnt + O::m_bPinPulled),
                                    &pinPulled, 1, nullptr);
                  snap.localNadePinPulled = pinPulled;
                  float tStr = 1.0f;
                  if (ReadProcessMemory(
                          hGame, (LPCVOID)(wpnEnt + O::m_flThrowStrength),
                          &tStr, sizeof(tStr), nullptr) &&
                      tStr >= 0.0f && tStr <= 1.0f) {
                    snap.localNadeThrowStrength = tStr;
                  }
                }
              }
            }
          }
        }
      }

      static int s_cacheSlotCursor = 1;
      static uint64_t s_lastC4CheckMs = 0;
      static uintptr_t s_cachedLocalCtrl = 0;
      static uint32_t s_cachedC4OwnerHandle = 0;
      uint64_t nowMs = GetTickCount64();

      if (nowMs - s_lastC4CheckMs >= 200) {
        s_lastC4CheckMs = nowMs;
        ReadProcessMemory(
            hGame, (LPCVOID)(clientBase + O::dwLocalPlayerController),
            &s_cachedLocalCtrl, sizeof(s_cachedLocalCtrl), nullptr);
        uintptr_t c4Wpn = 0;
        ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwWeaponC4), &c4Wpn,
                          sizeof(c4Wpn), nullptr);
        if (c4Wpn)
          ReadProcessMemory(hGame, (LPCVOID)(c4Wpn + O::m_hOwnerEntity),
                            &s_cachedC4OwnerHandle,
                            sizeof(s_cachedC4OwnerHandle), nullptr);
        else
          s_cachedC4OwnerHandle = 0;
      }

      uintptr_t listEntry = 0;
      ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10), &listEntry,
                        sizeof(listEntry), nullptr);
      if (listEntry) {
        if (s_cachedLocalCtrl &&
            (s_localPlayerIndex <= 0 || s_localPlayerIndex > 64)) {
          for (int slot = 1; slot <= 64; slot++) {
            uintptr_t ctrl = 0;
            ReadProcessMemory(hGame, (LPCVOID)(listEntry + slot * 0x78), &ctrl,
                              sizeof(ctrl), nullptr);
            if (ctrl && ctrl == s_cachedLocalCtrl) {
              s_localPlayerIndex = slot;
              break;
            }
          }
        }
        for (int i = 1; i <= 64; i++) {
          uintptr_t controller = 0;
          ReadProcessMemory(hGame, (LPCVOID)(listEntry + i * 0x78), &controller,
                            sizeof(controller), nullptr);
          if (!controller) {
            targets[i].active = false;
            continue;
          }
          if (s_cachedLocalCtrl && controller == s_cachedLocalCtrl)
            s_localPlayerIndex = i;
          uint32_t pawnHandle = 0;
          ReadProcessMemory(hGame, (LPCVOID)(controller + O::m_hPlayerPawn),
                            &pawnHandle, sizeof(pawnHandle), nullptr);
          if (!pawnHandle || pawnHandle == 0xFFFFFFFF) {
            targets[i].active = false;
            continue;
          }
          uintptr_t listEntry2 = 0;
          uintptr_t pawnEntryOff =
              entityList + 0x10 + 8 * ((pawnHandle & 0x7FFF) >> 9);
          ReadProcessMemory(hGame, (LPCVOID)pawnEntryOff, &listEntry2,
                            sizeof(listEntry2), nullptr);
          if (!listEntry2) {
            targets[i].active = false;
            continue;
          }
          uintptr_t pawn = 0;
          ReadProcessMemory(hGame,
                            (LPCVOID)(listEntry2 + 0x78 * (pawnHandle & 0x1FF)),
                            &pawn, sizeof(pawn), nullptr);
          if (!pawn) {
            targets[i].active = false;
            continue;
          }
          targets[i].pawn = pawn;
          targets[i].pawnHandle = pawnHandle;
          uint8_t teamRaw = 0;
          ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_iTeamNum), &teamRaw,
                            sizeof(teamRaw), nullptr);
          targets[i].team = teamRaw;
          if (targets[i].name[0] == '\0' ||
              (nowMs - targets[i].lastNameRefresh > 8000)) {
            char nameBuf[64] = {0};
            ReadProcessMemory(hGame, (LPCVOID)(controller + O::m_iszPlayerName),
                              nameBuf, sizeof(nameBuf) - 1, nullptr);
            strncpy(targets[i].name, (nameBuf[0] != '\0') ? nameBuf : "Enemy",
                    sizeof(targets[i].name) - 1);
            targets[i].name[sizeof(targets[i].name) - 1] = '\0';
            targets[i].lastNameRefresh = nowMs;
          }
          uintptr_t gsn = 0;
          ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pGameSceneNode), &gsn,
                            sizeof(gsn), nullptr);
          targets[i].gameSceneNode = gsn;
          uintptr_t boneArr = 0;
          if (gsn)
            ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_boneArray), &boneArr,
                              sizeof(boneArr), nullptr);
          targets[i].boneArrayAddress = boneArr;
          targets[i].active = true;
          {
            uintptr_t wpnSvc = 0;
            ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pWeaponSvc), &wpnSvc,
                              sizeof(wpnSvc), nullptr);
            if (wpnSvc) {
              uint32_t activeWpnH = 0;
              ReadProcessMemory(hGame, (LPCVOID)(wpnSvc + O::m_hActiveWpn),
                                &activeWpnH, sizeof(activeWpnH), nullptr);
              if (activeWpnH && activeWpnH != 0xFFFFFFFF) {
                if (activeWpnH != targets[i].cachedWpnHandle ||
                    targets[i].weaponName[0] == '\0') {
                  targets[i].cachedWpnHandle = activeWpnH;
                  uintptr_t wpnListEntry = 0;
                  ReadProcessMemory(hGame,
                                    (LPCVOID)(entityList + 0x10 +
                                              8 * ((activeWpnH & 0x7FFF) >> 9)),
                                    &wpnListEntry, sizeof(wpnListEntry),
                                    nullptr);
                  if (wpnListEntry) {
                    uintptr_t wpnEnt = 0;
                    ReadProcessMemory(
                        hGame,
                        (LPCVOID)(wpnListEntry + 0x70 * (activeWpnH & 0x1FF)),
                        &wpnEnt, sizeof(wpnEnt), nullptr);
                    if (wpnEnt) {
                      uint16_t defIdx = 0;
                      ReadProcessMemory(hGame,
                                        (LPCVOID)(wpnEnt + O::m_AttrMgr +
                                                  O::m_Item_off +
                                                  O::m_iItemDefIdx),
                                        &defIdx, sizeof(defIdx), nullptr);
                      const char *wn =
                          defIdx ? WeaponNameFromDefIdx(defIdx) : nullptr;
                      if (wn) {
                        strncpy(targets[i].weaponName, wn,
                                sizeof(targets[i].weaponName) - 1);
                        targets[i]
                            .weaponName[sizeof(targets[i].weaponName) - 1] =
                            '\0';
                      } else {
                        uintptr_t ident = 0, namePtr = 0;
                        ReadProcessMemory(hGame,
                                          (LPCVOID)(wpnEnt + O::m_pEntity_id),
                                          &ident, sizeof(ident), nullptr);
                        if (ident)
                          ReadProcessMemory(
                              hGame, (LPCVOID)(ident + O::m_designerName),
                              &namePtr, sizeof(namePtr), nullptr);
                        if (namePtr) {
                          char dnBuf[32] = {};
                          ReadProcessMemory(hGame, (LPCVOID)namePtr, dnBuf,
                                            sizeof(dnBuf) - 1, nullptr);
                          const char *p = strstr(dnBuf, "weapon_");
                          strncpy(targets[i].weaponName, p ? p + 7 : dnBuf,
                                  sizeof(targets[i].weaponName) - 1);
                          targets[i]
                              .weaponName[sizeof(targets[i].weaponName) - 1] =
                              '\0';
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          {
            bool isCarrier = (s_cachedC4OwnerHandle != 0 &&
                              s_cachedC4OwnerHandle != 0xFFFFFFFF &&
                              targets[i].pawnHandle == s_cachedC4OwnerHandle);
            if (!isCarrier &&
                (strstr(targets[i].weaponName, "c4") != nullptr ||
                 strstr(targets[i].weaponName, "C4") != nullptr)) {
              isCarrier = true;
            }
            targets[i].hasBomb = isCarrier;
          }
        }
      }

      // OPT: hoist the per-frame weapon-config scan out of the 64-slot entity loop.
      bool isAnyAimbotEnabled = aimbotEnabled || g_Config.weapons[WEAPON_GENERAL].enabled;
      for (int w = 0; !isAnyAimbotEnabled && w < WEAPON_COUNT; w++) {
        if (g_Config.weapons[w].enabled) isAnyAimbotEnabled = true;
      }
      int liveTargets = 0;
      for (int i = 1; i <= 64; i++) {
        if (!targets[i].active || !targets[i].pawn)
          continue;
        if (targets[i].pawn == localPawn)
          continue;
        // OPT: removed fast-pass teammate skip — render side is single source
        // of truth
        PawnBatchLow low{};
        if (!ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + 0x330), &low,
                               sizeof(PawnBatchLow), nullptr))
          continue;
        if (low.health <= 0 || low.health > 500) {
          s_prevHealth[i] = 0;
          s_lastSpottedMs[i] = 0; // OPT: clear stale hysteresis on death
          continue;
        }
        if (targets[i].team && targets[i].team != snap.localTeam &&
            low.health > 0) {
          if (s_prevHealth[i] > low.health && s_prevHealth[i] <= 100) {
            int dmg = s_prevHealth[i] - low.health;
            if (snap.localShotsFired > 0) {
              g_hitmarkerDamage.store(dmg, std::memory_order_relaxed);
              g_hitmarkerTime.store(nowMs, std::memory_order_release);
              if (g_Config.visuals.hitsounds) {
                static uint64_t s_lastBeepMs = 0;
                if (nowMs - s_lastBeepMs > 60) {
                  s_lastBeepMs = nowMs;
                  MessageBeep(MB_OK);
                }
              }
            }
          }
          s_prevHealth[i] = low.health;
        }
        PawnOrigin pOrg{};
        if (!ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_vOldOrigin),
                               &pOrg, sizeof(PawnOrigin), nullptr))
          continue;
        PawnSpot pSpot{};
        if (!ReadProcessMemory(hGame,
                               (LPCVOID)(targets[i].pawn + O::m_entitySpottedState),
                               &pSpot, sizeof(PawnSpot), nullptr))
          continue;

        uint64_t enemyMask = (uint64_t)pSpot.spottedByMask[0] |
                             ((uint64_t)pSpot.spottedByMask[1] << 32);
        bool spottedByLocal = false;
        if (s_localPlayerIndex > 0 && s_localPlayerIndex <= 64) {
          spottedByLocal =
              ((enemyMask & (1ULL << (s_localPlayerIndex - 1))) != 0) ||
              ((enemyMask & (1ULL << s_localPlayerIndex)) != 0);
        }
        bool localSpottedEnemy = false;
        if (i >= 0 && i <= 64) {
          localSpottedEnemy =
              ((snap.localSpottedMask & (1ULL << i)) != 0) ||
              (i > 0 && (snap.localSpottedMask & (1ULL << (i - 1))) != 0);
        }
        bool rawSpotted = false;
        if (g_Config.visuals.vis_check_mode == 0)
          rawSpotted = pSpot.spotted;
        else if (g_Config.visuals.vis_check_mode == 1)
          rawSpotted = spottedByLocal || localSpottedEnemy;
        else
          rawSpotted = pSpot.spotted || spottedByLocal || localSpottedEnemy;
        if (rawSpotted)
          s_lastSpottedMs[i] = nowMs;
        uint64_t hystMs =
            (uint64_t)std::clamp(g_Config.visuals.vis_hysteresis_ms, 0, 300);
        bool isSpotted = rawSpotted || (hystMs > 0 && s_lastSpottedMs[i] &&
                                        (nowMs - s_lastSpottedMs[i] <= hystMs));

        EntityData ed{};
        ed.valid = true;
        ed.hasBomb = targets[i].hasBomb;
        ed.spotted = isSpotted;
        ed.spottedByLocal = spottedByLocal;
        ed.pawn = targets[i].pawn;
        if (!targets[i].boneArrayAddress && low.gameSceneNode) {
          uintptr_t boneArr = 0;
          if (ReadProcessMemory(hGame, (LPCVOID)(low.gameSceneNode + O::m_boneArray), &boneArr, sizeof(boneArr), nullptr)) {
            targets[i].boneArrayAddress = boneArr;
          }
        }
        ed.boneArrayAddress = targets[i].boneArrayAddress;
        ed.health = low.health;
        ed.team = targets[i].team ? targets[i].team : low.team;
        ed.entitySlot = i;
        memcpy(ed.name, targets[i].name, sizeof(ed.name));
        ed.name[sizeof(ed.name) - 1] = '\0';
        memcpy(ed.weaponName, targets[i].weaponName, sizeof(ed.weaponName));
        ed.weaponName[sizeof(ed.weaponName) - 1] = '\0';
        ed.velocity = low.velocity;
        ed.origin = pOrg.origin;
        ed.armor = pSpot.armor;
        ed.shotsFired = pSpot.shotsFired;
        float sndTime = pSpot.emitSoundTime;
        if (sndTime != targets[i].cachedSoundTime && sndTime > 0.0f) {
          targets[i].cachedSoundTime = sndTime;
          uint64_t sNowMs = GetTickCount64();
          g_soundEvents[i].origin = pOrg.origin;
          g_soundEvents[i].team = targets[i].team ? targets[i].team : low.team;
          strncpy(g_soundEvents[i].name,
                  targets[i].name[0] ? targets[i].name : "Enemy",
                  sizeof(g_soundEvents[i].name) - 1);
          g_soundEvents[i].timestamp = sNowMs;
          g_soundWallMs[i].store(sNowMs, std::memory_order_release);
        }
        ed.lastSoundTime = sndTime;
        if (drawEyeRay) {
          ReadProcessMemory(hGame,
                            (LPCVOID)(targets[i].pawn + O::m_angEyeAngles),
                            &ed.eyeAngles, sizeof(ed.eyeAngles), nullptr);
        }
        ed.headPos = {ed.origin.x, ed.origin.y, ed.origin.z + 65.0f};
        float distSq = ed.origin.DistToSq(snap.localOrigin);
        // Read CS2 skeleton bones (23 bones covering head, spine, arms, hands, legs, feet)
        bool needFullBones = drawSkeletons;
        bool needHeadBone = (isAnyAimbotEnabled || drawHeadDot || triggerbotEnabled);
        if (targets[i].boneArrayAddress && (needFullBones || needHeadBone)) {
          if (needFullBones) {
            BoneJointData bj[23]{};
            if (ReadProcessMemory(hGame, (LPCVOID)targets[i].boneArrayAddress,
                                  bj, sizeof(bj), nullptr)) {
              ed.hasBones = true;
              for (int b = 0; b < 23; b++)
                ed.bones[b] = bj[b].pos;
              if (bj[7].pos.x || bj[7].pos.y || bj[7].pos.z)
                ed.headPos = bj[7].pos;
            }
          } else {
            BoneJointData headBone{};
            if (ReadProcessMemory(hGame,
                                  (LPCVOID)(targets[i].boneArrayAddress +
                                            7 * sizeof(BoneJointData)),
                                  &headBone, sizeof(headBone), nullptr)) {
              if (headBone.pos.x || headBone.pos.y || headBone.pos.z)
                ed.headPos = headBone.pos;
            }
            ed.bones[7] = ed.headPos;
            ed.bones[6] = {ed.headPos.x, ed.headPos.y, ed.headPos.z - 8.0f};
            ed.bones[4] = {ed.headPos.x, ed.headPos.y, ed.headPos.z - 22.0f};
            ed.bones[3] = {ed.headPos.x, ed.headPos.y, ed.headPos.z - 34.0f};
            ed.bones[1] = {ed.headPos.x, ed.headPos.y, ed.headPos.z - 44.0f};
            ed.hasBones = true;
          }
        }
        if (snap.count < 128)
          snap.entities[snap.count++] = ed;
        liveTargets++;
      }
      telemetryTargets.store(liveTargets);
    }

    snap.localPlayerIndex = s_localPlayerIndex;
    snap.timestamp = std::chrono::high_resolution_clock::now();

    auto tEnd = std::chrono::high_resolution_clock::now();
    telemetryScanMs.store(
        std::chrono::duration<float, std::milli>(tEnd - tStart).count());

    static auto s_lastHb = std::chrono::steady_clock::now();
    auto nowHb = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(nowHb - s_lastHb)
            .count() >= 4) {
      int enemies = 0, teammates = 0;
      for (int i = 0; i < snap.count; i++) {
        if (!snap.entities[i].valid || snap.entities[i].health <= 0)
          continue;
        if (snap.localTeam && snap.entities[i].team == snap.localTeam)
          teammates++;
        else
          enemies++;
      }
      const char *teamStr = (snap.localTeam == 2)   ? "T"
                            : (snap.localTeam == 3) ? "CT"
                                                    : "SPEC";
      ConsoleLogger::Log(ConsoleLogger::LOG_INFO,
                         "Telemetry: %d Entities (%d Enemies, %d Team) | Team: "
                         "%s | Pos: (%.0f, %.0f, %.0f) | FrameScan: %.2f ms",
                         snap.count, enemies, teammates, teamStr,
                         snap.localOrigin.x, snap.localOrigin.y,
                         snap.localOrigin.z, telemetryScanMs.load());
      s_lastHb = nowHb;
    }

    {
      int writeSlot = g_snapInFlight.load(std::memory_order_relaxed);
      std::swap(g_snapBuf[writeSlot], snap);
      g_snapWriteIdx.store(writeSlot, std::memory_order_release);
      int readSlot = g_snapRenderReading.load(std::memory_order_acquire);
      int nextWrite = 0;
      for (int s = 0; s < 3; s++) {
        if (s != writeSlot && s != readSlot) {
          nextWrite = s;
          break;
        }
      }
      g_snapInFlight.store(nextWrite, std::memory_order_relaxed);
    }

    static HighPrecisionSleep s_workerTimer;
    s_workerTimer.SleepMicros(1000);
  }
}

// ============ SLOW SCAN THREAD ============
void SlowScanThread() {
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
  while (running) {
    if (!hGame || !clientBase) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      continue;
    }
    DWORD exitCode = 0;
    if (GetExitCodeProcess(hGame, &exitCode) && exitCode != STILL_ACTIVE) {
      ConsoleLogger::Log(ConsoleLogger::LOG_WARN,
                         "Counter-Strike 2 process exited (Exit Code: %lu). "
                         "Waiting for restart...",
                         exitCode);
      CloseHandle(hGame);
      hGame = NULL;
      clientBase = 0;
      pid = 0;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      continue;
    }

    SlowSnapshot slow{};
    uintptr_t c4Ent = 0;
    float curTime = 0.0f;
    {
      uintptr_t c4Ptr = 0;
      ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwPlantedC4), &c4Ptr,
                        sizeof(c4Ptr), nullptr);
      if (c4Ptr) {
        bool ticking = false;
        ReadProcessMemory(hGame, (LPCVOID)(c4Ptr + O::c4_bTicking), &ticking, 1,
                          nullptr);
        if (ticking)
          c4Ent = c4Ptr;
        else {
          uintptr_t deref = 0;
          ReadProcessMemory(hGame, (LPCVOID)c4Ptr, &deref, sizeof(deref),
                            nullptr);
          if (deref) {
            ReadProcessMemory(hGame, (LPCVOID)(deref + O::c4_bTicking),
                              &ticking, 1, nullptr);
            if (ticking)
              c4Ent = deref;
          }
        }
      }
      uintptr_t gvPtr = 0;
      ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwGlobalVars), &gvPtr,
                        sizeof(gvPtr), nullptr);
      if (gvPtr) {
        ReadProcessMemory(hGame, (LPCVOID)(gvPtr + 0x2C), &curTime,
                          sizeof(curTime), nullptr);
        if (curTime <= 0.0f || curTime > 1000000.0f) {
          ReadProcessMemory(hGame,
                            (LPCVOID)(clientBase + O::dwGlobalVars + 0x2C),
                            &curTime, sizeof(curTime), nullptr);
        }
        uintptr_t mapNamePtr = 0;
        ReadProcessMemory(hGame, (LPCVOID)(gvPtr + 0x188), &mapNamePtr,
                          sizeof(mapNamePtr), nullptr);
        char mapBuf[32] = {};
        if (mapNamePtr)
          ReadProcessMemory(hGame, (LPCVOID)mapNamePtr, mapBuf,
                            sizeof(mapBuf) - 1, nullptr);
        if (mapBuf[0] == '\0') {
          uintptr_t mapPtr = 0;
          ReadProcessMemory(hGame, (LPCVOID)(gvPtr + 0x180), &mapPtr,
                            sizeof(mapPtr), nullptr);
          if (mapPtr)
            ReadProcessMemory(hGame, (LPCVOID)mapPtr, mapBuf,
                              sizeof(mapBuf) - 1, nullptr);
        }
        if (mapBuf[0] != '\0') {
          strncpy(slow.currentMapName, mapBuf, sizeof(slow.currentMapName) - 1);
          std::string m(mapBuf);
          std::transform(m.begin(), m.end(), m.begin(), ::tolower);
          if (m.find("mirage") != std::string::npos)
            slow.detectedMapIdx = 0;
          else if (m.find("inferno") != std::string::npos)
            slow.detectedMapIdx = 1;
          else if (m.find("dust2") != std::string::npos ||
                   m.find("dust") != std::string::npos)
            slow.detectedMapIdx = 2;
          else if (m.find("nuke") != std::string::npos)
            slow.detectedMapIdx = 3;
          else if (m.find("anubis") != std::string::npos)
            slow.detectedMapIdx = 4;
          else if (m.find("ancient") != std::string::npos)
            slow.detectedMapIdx = 5;
          else if (m.find("vertigo") != std::string::npos)
            slow.detectedMapIdx = 6;
          else if (m.find("overpass") != std::string::npos)
            slow.detectedMapIdx = 7;
          selectedMapIdx = slow.detectedMapIdx;
        }
      }
      slow.bomb.curTime = curTime;
      if (c4Ent) {
        bool ticking = false;
        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bTicking), &ticking, 1,
                          nullptr);
        slow.bomb.ticking = ticking;
        if (ticking) {
          slow.bomb.valid = true;
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_nSite),
                            &slow.bomb.site, sizeof(slow.bomb.site), nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flTimer),
                            &slow.bomb.timerLength,
                            sizeof(slow.bomb.timerLength), nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flBlow),
                            &slow.bomb.blowTime, sizeof(slow.bomb.blowTime),
                            nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bDefusing),
                            &slow.bomb.defusing, 1, nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flDefuseLen),
                            &slow.bomb.defuseLength,
                            sizeof(slow.bomb.defuseLength), nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flDefuseCountDown),
                            &slow.bomb.defuseCountDown,
                            sizeof(slow.bomb.defuseCountDown), nullptr);
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bDefused),
                            &slow.bomb.defused, 1, nullptr);
          if (curTime > 0.0f && slow.bomb.blowTime > curTime)
            slow.bomb.remainingBlow = slow.bomb.blowTime - curTime;
          else
            slow.bomb.remainingBlow = slow.bomb.timerLength;
          if (slow.bomb.defusing && curTime > 0.0f &&
              slow.bomb.defuseCountDown > curTime) {
            slow.bomb.remainingDefuse = slow.bomb.defuseCountDown - curTime;
            slow.bomb.canDefuseInTime =
                (slow.bomb.defuseCountDown <= slow.bomb.blowTime);
          } else {
            slow.bomb.remainingDefuse = 0.0f;
            slow.bomb.canDefuseInTime = true;
          }
          slow.bomb.captureTime = std::chrono::steady_clock::now();
          uintptr_t gsn = 0;
          ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::m_pGameSceneNode), &gsn,
                            sizeof(gsn), nullptr);
          if (gsn)
            ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_vecAbsOrigin),
                              &slow.bomb.worldPos, sizeof(slow.bomb.worldPos),
                              nullptr);
        }
      }
      static std::chrono::steady_clock::time_point s_plantWallTime;
      static bool s_wallTimeCaptured = false;
      if (slow.bomb.valid && slow.bomb.ticking) {
        if (s_wallTimeCaptured) {
          slow.bomb.plantWallTime = s_plantWallTime;
          slow.bomb.wallTimeCaptured = true;
        } else {
          s_plantWallTime = std::chrono::steady_clock::now();
          slow.bomb.plantWallTime = s_plantWallTime;
          slow.bomb.wallTimeCaptured = true;
          s_wallTimeCaptured = true;
        }
      } else {
        slow.bomb.wallTimeCaptured = false;
        s_wallTimeCaptured = false;
      }
    }

    struct CachedEntityIdentity {
      uintptr_t ent = 0;
      bool isWpn = false;
      bool isProj = false;
      int pType = 0;
      char pName[16] = {};
      char cleanName[32] = {};
      bool isC4 = false;
    };
    static CachedEntityIdentity s_entCache[1024];

    if (worldLoot || droppedC4Esp || bombEsp || liveNades || nadeTrajectory) {
      uintptr_t entityList = 0;
      ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList),
                        &entityList, sizeof(entityList), nullptr);
      if (entityList) {
        for (int chunkIdx = 0; chunkIdx < 2; chunkIdx++) {
          if (slow.itemCount >= 60 && slow.projectileCount >= 16)
            break;
          uintptr_t chunkPtr = 0;
          ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10 + 8 * chunkIdx),
                            &chunkPtr, sizeof(chunkPtr), nullptr);
          if (!chunkPtr)
            continue;
          int startSlot = (chunkIdx == 0) ? 64 : 0;
          for (int batchStart = startSlot;
               batchStart < 512 &&
               (slow.itemCount < 60 || slow.projectileCount < 16);
               batchStart += 64) {
            int batchCount = std::min(64, 512 - batchStart);
            BYTE slotBuf[64 * 0x78];
            size_t stride = 0x78;
            if (!ReadProcessMemory(hGame,
                                   (LPCVOID)(chunkPtr + 0x78 * batchStart),
                                   slotBuf, batchCount * 0x78, nullptr)) {
              stride = 0x70;
              if (!ReadProcessMemory(hGame,
                                     (LPCVOID)(chunkPtr + 0x70 * batchStart),
                                     slotBuf, batchCount * 0x70, nullptr))
                break;
            }
            for (int s = 0; s < batchCount; s++) {
              uintptr_t ent =
                  *reinterpret_cast<uintptr_t *>(slotBuf + s * stride);
              int globalSlot = chunkIdx * 512 + batchStart + s;
              if (!ent) {
                if (globalSlot < 1024) s_entCache[globalSlot].ent = 0;
                continue;
              }
              CachedEntityIdentity id{};
              if (globalSlot < 1024 && s_entCache[globalSlot].ent == ent) {
                id = s_entCache[globalSlot];
              } else {
                id.ent = ent;
                uintptr_t ident = 0;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_pEntity_id), &ident,
                                  sizeof(ident), nullptr);
                if (ident) {
                  uintptr_t namePtr = 0;
                  ReadProcessMemory(hGame, (LPCVOID)(ident + O::m_designerName),
                                    &namePtr, sizeof(namePtr), nullptr);
                  if (namePtr) {
                    char dnBuf[48] = {};
                    ReadProcessMemory(hGame, (LPCVOID)namePtr, dnBuf,
                                      sizeof(dnBuf) - 1, nullptr);
                    id.isWpn = (strncmp(dnBuf, "weapon_", 7) == 0);
                    id.isProj = (strstr(dnBuf, "_projectile") != nullptr ||
                                 strcmp(dnBuf, "inferno") == 0);
                    id.isC4 = (strcmp(dnBuf, "weapon_c4") == 0 ||
                               strstr(dnBuf, "c4") != nullptr);
                    if (id.isProj) {
                      id.pType = 0;
                      strncpy(id.pName, "GRENADE", sizeof(id.pName) - 1);
                      if (strstr(dnBuf, "smoke")) {
                        id.pType = 2;
                        strncpy(id.pName, "SMOKE", sizeof(id.pName) - 1);
                      } else if (strstr(dnBuf, "molotov") || strstr(dnBuf, "incendiary") || strcmp(dnBuf, "inferno") == 0) {
                        id.pType = 4;
                        strncpy(id.pName, "MOLOTOV", sizeof(id.pName) - 1);
                      } else if (strstr(dnBuf, "flash")) {
                        id.pType = 1;
                        strncpy(id.pName, "FLASH", sizeof(id.pName) - 1);
                      } else if (strstr(dnBuf, "he")) {
                        id.pType = 3;
                        strncpy(id.pName, "HE NADE", sizeof(id.pName) - 1);
                      } else if (strstr(dnBuf, "decoy")) {
                        id.pType = 5;
                        strncpy(id.pName, "DECOY", sizeof(id.pName) - 1);
                      }
                    } else if (id.isWpn) {
                      uint16_t defIdx = 0;
                      ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_AttrMgr + O::m_Item_off + O::m_iItemDefIdx), &defIdx, sizeof(defIdx), nullptr);
                      const char *cleanName = defIdx ? WeaponNameFromDefIdx(defIdx) : nullptr;
                      const char *displayP = cleanName ? cleanName : dnBuf + 7;
                      strncpy(id.cleanName, displayP, sizeof(id.cleanName) - 1);
                    }
                  }
                }
                if (globalSlot < 1024) s_entCache[globalSlot] = id;
              }

              if (!id.isWpn && !id.isProj)
                continue;
              if (id.isProj && slow.projectileCount < 16) {
                bool explodeBegan = false;
                ReadProcessMemory(hGame,
                                  (LPCVOID)(ent + O::m_bExplodeEffectBegan),
                                  &explodeBegan, 1, nullptr);
                if (explodeBegan && id.pType != 4 && id.pType != 2)
                  continue;
                uintptr_t gsn = 0;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_pGameSceneNode),
                                  &gsn, sizeof(gsn), nullptr);
                if (!gsn)
                  continue;
                Vec3 pos{};
                ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_vecAbsOrigin),
                                  &pos, sizeof(pos), nullptr);
                if (!pos.x && !pos.y && !pos.z)
                  continue;
                Vec3 vel{};
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_absVelocity),
                                  &vel, sizeof(vel), nullptr);
                if (vel.x == 0.0f && vel.y == 0.0f && vel.z == 0.0f) {
                  ReadProcessMemory(hGame,
                                    (LPCVOID)(ent + O::m_vInitialVelocity),
                                    &vel, sizeof(vel), nullptr);
                }
                int bounces = 0;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_nBounces),
                                  &bounces, sizeof(bounces), nullptr);
                float spawnTime = 0.0f;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_flSpawnTime),
                                  &spawnTime, sizeof(spawnTime), nullptr);
                ProjectileItem &proj = slow.projectiles[slow.projectileCount++];
                proj.valid = true;
                proj.projectileType = id.pType;
                proj.position = pos;
                proj.velocity = vel;
                proj.bounces = bounces;
                proj.spawnTime = spawnTime;
                proj.exploded = explodeBegan;
                strncpy(proj.name, id.pName, sizeof(proj.name) - 1);
                struct CUtlVecHead {
                  int size;
                  uintptr_t data;
                };
                CUtlVecHead trailVec{};
                if (ReadProcessMemory(hGame,
                                      (LPCVOID)(ent + O::m_arrTrajectoryPoints),
                                      &trailVec, sizeof(trailVec), nullptr)) {
                  if (trailVec.size > 1 && trailVec.size <= 64 &&
                      trailVec.data) {
                    proj.trailCount = trailVec.size;
                    ReadProcessMemory(hGame, (LPCVOID)trailVec.data,
                                      proj.trailPoints,
                                      trailVec.size * sizeof(Vec3), nullptr);
                  }
                }
              }
              if (id.isWpn && slow.itemCount < 60) {
                uint32_t ownerH = 0;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_hOwnerEntity),
                                  &ownerH, sizeof(ownerH), nullptr);
                if (ownerH != 0 && ownerH != 0xFFFFFFFF) {
                  int ownerSlot = ownerH & 0x1FF;
                  if (ownerSlot > 0 && ownerSlot <= 64)
                    continue;
                }
                uintptr_t gsn = 0;
                ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_pGameSceneNode),
                                  &gsn, sizeof(gsn), nullptr);
                if (!gsn)
                  continue;
                Vec3 pos{};
                ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_vecAbsOrigin),
                                  &pos, sizeof(pos), nullptr);
                if (!pos.x && !pos.y && !pos.z)
                  continue;
                WorldItem &item = slow.items[slow.itemCount++];
                item.valid = true;
                item.position = pos;
                item.isC4 = id.isC4;
                item.isProjectile = false;
                strncpy(item.name, id.cleanName[0] ? id.cleanName : "Weapon", sizeof(item.name) - 1);
              }
            }
          }
        }
      }
    }

    if (showSpectators) {
      int pubSlot = g_snapWriteIdx.load(std::memory_order_acquire);
      const auto &lastSnap = g_snapBuf[pubSlot];
      uintptr_t localPawn = lastSnap.localPawnAddr;
      uintptr_t localCtrl = lastSnap.localCtrlAddr;
      uint32_t localPawnHandle = lastSnap.localPawnHandle;
      uintptr_t entityList = lastSnap.entityListAddr;
      if (!entityList) {
        ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList),
                          &entityList, sizeof(entityList), nullptr);
      }
      if ((localPawn || localPawnHandle) && entityList) {
        uintptr_t listEntry = 0;
        ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10), &listEntry,
                          sizeof(listEntry), nullptr);
        if (listEntry) {
          BYTE ctrlBlock[64 * 0x70]{};
          if (ReadProcessMemory(hGame, (LPCVOID)(listEntry + 0x70), ctrlBlock,
                                sizeof(ctrlBlock), nullptr)) {
            for (int i = 1; i <= 64 && slow.spectatorCount < 8; i++) {
              uintptr_t ctrl =
                  *reinterpret_cast<uintptr_t *>(ctrlBlock + (i - 1) * 0x70);
              if (!ctrl || ctrl == localCtrl)
                continue;
              uint32_t pawnH = 0;
              ReadProcessMemory(hGame, (LPCVOID)(ctrl + O::m_hPlayerPawn),
                                &pawnH, sizeof(pawnH), nullptr);
              if (!pawnH || pawnH == 0xFFFFFFFF || pawnH == localPawnHandle)
                continue;
              uintptr_t ple = 0;
              ReadProcessMemory(
                  hGame,
                  (LPCVOID)(entityList + 0x10 + 8 * ((pawnH & 0x7FFF) >> 9)),
                  &ple, sizeof(ple), nullptr);
              if (!ple)
                continue;
              uintptr_t pawn = 0;
              ReadProcessMemory(hGame, (LPCVOID)(ple + 0x70 * (pawnH & 0x1FF)),
                                &pawn, sizeof(pawn), nullptr);
              if (!pawn || pawn == localPawn)
                continue;
              uintptr_t obsSvc = 0;
              ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pObserverSvc),
                                &obsSvc, sizeof(obsSvc), nullptr);
              if (!obsSvc)
                continue;
              uint32_t obsTgtH = 0;
              ReadProcessMemory(hGame, (LPCVOID)(obsSvc + O::m_hObsTgt),
                                &obsTgtH, sizeof(obsTgtH), nullptr);
              if (!obsTgtH || obsTgtH == 0xFFFFFFFF)
                ReadProcessMemory(hGame, (LPCVOID)(obsSvc + 0x44), &obsTgtH,
                                  sizeof(obsTgtH), nullptr);
              if (!obsTgtH || obsTgtH == 0xFFFFFFFF)
                continue;
              bool isSpectatingLocal = false;
              if (localPawnHandle &&
                  (obsTgtH & 0x7FFF) == (localPawnHandle & 0x7FFF))
                isSpectatingLocal = true;
              else if (localPawn) {
                uintptr_t tple = 0;
                ReadProcessMemory(hGame,
                                  (LPCVOID)(entityList + 0x10 +
                                            8 * ((obsTgtH & 0x7FFF) >> 9)),
                                  &tple, sizeof(tple), nullptr);
                if (tple) {
                  uintptr_t tPawn = 0;
                  ReadProcessMemory(hGame,
                                    (LPCVOID)(tple + 0x70 * (obsTgtH & 0x1FF)),
                                    &tPawn, sizeof(tPawn), nullptr);
                  if (tPawn == localPawn)
                    isSpectatingLocal = true;
                }
              }
              if (!isSpectatingLocal)
                continue;
              char nb[64] = {};
              ReadProcessMemory(hGame, (LPCVOID)(ctrl + O::m_iszPlayerName), nb,
                                sizeof(nb) - 1, nullptr);
              if (nb[0])
                strncpy(slow.spectators[slow.spectatorCount++].name, nb, 63);
            }
          }
        }
      }
    }
    int w = g_slowRing.writeIdx.load(std::memory_order_relaxed);
    int pub = g_slowRing.publishedIdx.load(std::memory_order_acquire);
    int next = 0;
    while (next == w || next == pub) next++;
    g_slowRing.data[w] = slow;
    g_slowRing.writeIdx.store(next, std::memory_order_release);
    g_slowRing.publishedIdx.store(w, std::memory_order_release);

    int scanSleepMs = (liveNades || nadeTrajectory) ? (80 + rand() % 15)
                                                    : (470 + rand() % 61);
    std::this_thread::sleep_for(std::chrono::milliseconds(scanSleepMs));
  }
}

// ============ DRIVER INPUT ============
static inline void DriverMouseMove(long dx, long dy) {
  if (dx == 0 && dy == 0)
    return;
  const std::int32_t rel_x = static_cast<std::int32_t>(dx);
  const std::int32_t rel_y = static_cast<std::int32_t>(dy);
  if (matchio::g_InputDevice.is_open() &&
      matchio::g_InputDevice.mouse_move_relative(rel_x, rel_y))
    return;
  mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(rel_x),
              static_cast<DWORD>(rel_y), 0, 0);
}
static inline void DriverMouseClick() {
  if (!matchio::g_InputDevice.is_open() ||
      !matchio::g_InputDevice.mouse_click(matchio::mouse_button::left)) {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
  }
}
static inline void DriverKeyDown(uint16_t scanCode) {
  if (matchio::g_InputDevice.is_open() &&
      matchio::g_InputDevice.key_down(scanCode))
    return;
  INPUT in{};
  in.type = INPUT_KEYBOARD;
  in.ki.wScan = scanCode;
  in.ki.dwFlags = KEYEVENTF_SCANCODE;
  SendInput(1, &in, sizeof(INPUT));
}
static inline void DriverKeyUp(uint16_t scanCode) {
  if (matchio::g_InputDevice.is_open() &&
      matchio::g_InputDevice.key_up(scanCode))
    return;
  INPUT in{};
  in.type = INPUT_KEYBOARD;
  in.ki.wScan = scanCode;
  in.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
  SendInput(1, &in, sizeof(INPUT));
}
static inline void DriverKeyTap(uint16_t scanCode) {
  if (matchio::g_InputDevice.is_open() &&
      matchio::g_InputDevice.key_tap(scanCode))
    return;
  DriverKeyDown(scanCode);
  DriverKeyUp(scanCode);
}

// ============ AIMBOT (uses FrameContext) ============
static void AimbotTick(const FrameContext &ctx) {
  if (!g_masterEnabled || menuOpen)
    return;
  if (pid && ctx.foregroundPid != pid && ctx.foregroundHwnd != hwndOverlay)
    return;

  const WeaponConfig &wcfg = (*ctx.wcfg).enabled 
                                 ? *ctx.wcfg 
                                 : (g_Config.weapons[WEAPON_GENERAL].enabled 
                                        ? g_Config.weapons[WEAPON_GENERAL] 
                                        : *ctx.wcfg);
  if (!wcfg.enabled && !g_Config.weapons[WEAPON_GENERAL].enabled)
    return;

  float sens =
      (g_gameSens >= 0.05f && g_gameSens <= 25.0f) ? g_gameSens : 1.25f;
  float focalLen = (float)ctx.screenH * 0.666667f;
  if (focalLen < 100.0f)
    focalLen = 720.0f;

  static float s_aimRemainderX = 0.0f, s_aimRemainderY = 0.0f;

  bool keyHeld = false;
  if (wcfg.type == 2)
    keyHeld = true;
  else if (wcfg.type == 0)
    keyHeld = ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
  else {
    int vk = (wcfg.aim_key > 0) ? wcfg.aim_key
                                : (g_aimKeyVk > 0 ? g_aimKeyVk : VK_LMENU);
    keyHeld = ((GetAsyncKeyState(vk) & 0x8000) != 0);
    if (!keyHeld) {
      if (vk == VK_MENU)
        keyHeld = ((GetAsyncKeyState(VK_LMENU) & 0x8000) != 0) ||
                  ((GetAsyncKeyState(VK_RMENU) & 0x8000) != 0);
      else if (vk == VK_SHIFT)
        keyHeld = ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0) ||
                  ((GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0);
      else if (vk == VK_CONTROL)
        keyHeld = ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0) ||
                  ((GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0);
      else if (vk == VK_XBUTTON1 || vk == 0x05)
        keyHeld = ((GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0);
      else if (vk == VK_XBUTTON2 || vk == 0x06)
        keyHeld = ((GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0);
      else if (vk == VK_RBUTTON || vk == 0x02)
        keyHeld = ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0);
    }
  }
  static int s_lockedTargetSlot = -1;
  static Vec3 s_standaloneOldPunch{0.0f, 0.0f, 0.0f};
  if (!keyHeld) {
    s_aimRemainderX = 0.0f;
    s_aimRemainderY = 0.0f;
    s_lockedTargetSlot = -1;

    // Standalone RCS when not holding aim key but holding left click
    if (wcfg.rcs_enabled && wcfg.rcs_standalone && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
      int startBullet = (wcfg.rcs_start_bullet > 0 ? wcfg.rcs_start_bullet : 1);
      const auto &snap = *ctx.snap;
      if (snap.localShotsFired >= startBullet) {
        Vec3 curPunch = snap.localPunchAngle;
        Vec3 punchDelta = {curPunch.x - s_standaloneOldPunch.x, curPunch.y - s_standaloneOldPunch.y, 0.0f};
        float deltaYaw = -punchDelta.y * (wcfg.rcs_yaw / 100.0f) * kRcsPunchScale;
        float deltaPitch = punchDelta.x * (wcfg.rcs_pitch / 100.0f) * kRcsPunchScale;
        float countsX = deltaYaw / (0.022f * sens);
        float countsY = deltaPitch / (0.022f * sens);
        long dx = (long)std::lroundf(countsX);
        long dy = (long)std::lroundf(countsY);
        if (dx || dy)
          DriverMouseMove(dx, dy);
        s_standaloneOldPunch = curPunch;
        return;
      }
    }
    s_standaloneOldPunch = ctx.snap->localPunchAngle;
    return;
  }
  s_standaloneOldPunch = ctx.snap->localPunchAngle;

  float cx = ctx.screenW / 2.0f, cy = ctx.screenH / 2.0f;
  float fovDeg = (wcfg.fov > 0.1f) ? wcfg.fov : 5.0f;
  float fovPx = fovDeg * ((float)ctx.screenH / 90.0f);
  float bestScore = 999999.0f;
  Vec2 bestScr = {cx, cy};
  Vec3 bestWorld{}, bestVel{};
  EntityData bestEnt{};
  bool found = false;
  int targetBone = (wcfg.hitbox == 0)   ? 7   // Head (Bone 7)
                   : (wcfg.hitbox == 1) ? 6   // Neck (Bone 6)
                   : (wcfg.hitbox == 2) ? 4   // Chest (Bone 4 - spine_2)
                                        : 1;  // Pelvis (Bone 1 - pelvis)

  auto getTargetWorld = [&](const EntityData &ent) -> Vec3 {
    if (ent.hasBones && (ent.bones[targetBone].x != 0.0f || ent.bones[targetBone].y != 0.0f || ent.bones[targetBone].z != 0.0f)) {
      if (ent.headPos.x != 0.0f) {
        float dSq = ent.bones[targetBone].DistToSq(ent.headPos);
        if (dSq < 8100.0f)
          return ent.bones[targetBone];
      } else {
        return ent.bones[targetBone];
      }
    }
    // Reliable procedural fallback based on head position
    if (targetBone == 6) return {ent.headPos.x, ent.headPos.y, ent.headPos.z - 7.5f};
    if (targetBone == 4) return {ent.headPos.x, ent.headPos.y, ent.headPos.z - 19.0f};
    if (targetBone == 1) return {ent.headPos.x, ent.headPos.y, ent.headPos.z - 35.0f};
    return ent.headPos;
  };

  const auto &snap = *ctx.snap;

  // 1. Try to maintain current locked target if still valid & within expanded FOV
  if (s_lockedTargetSlot > 0 && s_lockedTargetSlot <= 64) {
    for (int e = 0; e < snap.count; e++) {
      const auto &ent = snap.entities[e];
      if (ent.entitySlot == s_lockedTargetSlot && ent.valid && ent.health > 0) {
        if (!teamCheck || !ctx.localTeam || ent.team != ctx.localTeam) {
          if (!wcfg.cond_visible || ent.spotted || ent.spottedByLocal) {
            Vec3 aimWorld = getTargetWorld(ent);
            Vec2 aimScr;
            if (aimWorld.x != 0.0f || aimWorld.y != 0.0f || aimWorld.z != 0.0f) {
              if (WorldToScreen(aimWorld, *ctx.viewMatrix, aimScr)) {
                float dx = aimScr.x - cx, dy = aimScr.y - cy;
                float dist2D = std::sqrt(dx * dx + dy * dy);
                if (dist2D <= fovPx * 1.35f) {
                  bestScr = aimScr;
                  bestWorld = aimWorld;
                  bestVel = ent.velocity;
                  bestEnt = ent;
                  found = true;
                }
              }
            }
          }
        }
        break;
      }
    }
  }

  // 2. If locked target is lost or invalid, select best target in FOV
  if (!found) {
    s_lockedTargetSlot = -1;
    for (int e = 0; e < snap.count; e++) {
      const auto &ent = snap.entities[e];
      if (!ent.valid || ent.health <= 0)
        continue;
      if (teamCheck && ctx.localTeam && ent.team == ctx.localTeam)
        continue;
      if (wcfg.cond_visible && !ent.spotted && !ent.spottedByLocal)
        continue;
      if (wcfg.min_damage > 1 && ent.health < wcfg.min_damage)
        continue;
      Vec3 aimWorld = getTargetWorld(ent);
      if (!aimWorld.x && !aimWorld.y && !aimWorld.z)
        continue;
      Vec2 aimScr;
      if (!WorldToScreen(aimWorld, *ctx.viewMatrix, aimScr))
        continue;
      float dx = aimScr.x - cx, dy = aimScr.y - cy;
      float dist2D = std::sqrt(dx * dx + dy * dy);
      if (dist2D > fovPx)
        continue;
      float score = dist2D;
      if (wcfg.priority == 1)
        score = ctx.localOrigin.DistTo(ent.origin);
      else if (wcfg.priority == 2)
        score = (float)ent.health * 100.0f + dist2D;
      if (score < bestScore) {
        bestScore = score;
        bestScr = aimScr;
        bestWorld = aimWorld;
        bestVel = ent.velocity;
        bestEnt = ent;
        found = true;
      }
    }
    if (found) {
      s_lockedTargetSlot = bestEnt.entitySlot;
    }
  }

  static auto s_lastAimTick = std::chrono::steady_clock::now();
  auto aimNow = std::chrono::steady_clock::now();
  float dt = std::chrono::duration<float>(aimNow - s_lastAimTick).count();
  s_lastAimTick = aimNow;
  if (dt <= 0.0f || dt > 0.1f)
    dt = 0.005f;

  float stepX = 0.0f, stepY = 0.0f;
  if (found) {
    if (aimbotLead) {
      float dist3d = ctx.localOrigin.DistTo(bestWorld);
      // Per-weapon projectile speed (units/sec) for a physically sane lead.
      float bulletSpeed = 2500.0f;
      switch (snap.localWeaponDefIdx) {
      case 9: case 38: case 40: bulletSpeed = 4000.0f; break;   // AWP / SCAR-20 / SSG08
      case 7: case 8: case 10: case 13: case 39: bulletSpeed = 3000.0f; break; // AK/AUG/FAMAS/Galil/SG553
      case 16: case 60: bulletSpeed = 2700.0f; break;           // M4A4 / M4A1-S
      default: bulletSpeed = 2500.0f; break;
      }
      float tof = dist3d / bulletSpeed;
      // Two-pass iterative time-of-flight refinement (target keeps moving).
      for (int _it = 0; _it < 2; _it++) {
        Vec3 _pred = bestWorld + bestVel * tof;
        tof = ctx.localOrigin.DistTo(_pred) / bulletSpeed;
      }
      tof = std::clamp(tof, 0.0f, 0.35f);
      Vec3 ledWorld = bestWorld + bestVel * tof;
      Vec2 ledScr;
      if (WorldToScreen(ledWorld, *ctx.viewMatrix, ledScr))
        bestScr = ledScr;
    }
    float movX = (bestScr.x - cx), movY = (bestScr.y - cy);
    if (wcfg.rcs_enabled && snap.localShotsFired >= (wcfg.rcs_start_bullet > 0 ? wcfg.rcs_start_bullet : 1)) {
      float punchX = snap.localPunchAngle.y * (wcfg.rcs_yaw / 100.0f) * kRcsPunchScale;
      float punchY = snap.localPunchAngle.x * (wcfg.rcs_pitch / 100.0f) * kRcsPunchScale;
      float rcsPxX = (punchX * (3.14159265f / 180.0f)) * focalLen;
      float rcsPxY = (punchY * (3.14159265f / 180.0f)) * focalLen;
      movX -= rcsPxX;
      movY += rcsPxY;
    }
    float targetDist = std::sqrt(movX * movX + movY * movY);

    // Sub-pixel deadband: if crosshair is within 2.0px, stop to prevent oscillation
    if (targetDist < 2.0f) {
      s_aimRemainderX = 0.0f;
      s_aimRemainderY = 0.0f;
      return;
    }

    // Rate-based exponential smoothing (completely frame-rate independent)
    float alpha = 1.0f;
    if (wcfg.smoothing) {
      float strength = (wcfg.smooth_strength >= 1.0f) ? wcfg.smooth_strength : 1.0f;
      float smoothRate = 32.0f / strength; // target tracking rate in Hz

      if (wcfg.smooth_type == 1) { // Smooth curve
        float norm = std::clamp(targetDist / (fovPx > 0.01f ? fovPx : 1.0f), 0.05f, 1.0f);
        float curve = 1.0f - std::pow(1.0f - norm, 2.2f);
        smoothRate *= (0.45f + 0.85f * curve);
      } else if (wcfg.smooth_type == 2) { // Exponential
        float norm = std::clamp(targetDist / (fovPx > 0.01f ? fovPx : 1.0f), 0.02f, 1.0f);
        smoothRate *= (0.35f + std::pow(norm, 1.3f) * 1.25f);
      }

      if (wcfg.smooth_humanize > 0.05f) {
        float humNorm = std::clamp(wcfg.smooth_humanize / 30.0f, 0.0f, 1.0f);
        smoothRate *= (1.0f - 0.15f * humNorm);
      }

      alpha = 1.0f - std::exp(-smoothRate * dt);
      alpha = std::clamp(alpha, 0.02f, 0.85f);
    }

    float deltaYawDeg = std::atan2(movX, focalLen) * (180.0f / 3.14159265f);
    float deltaPitchDeg = std::atan2(movY, focalLen) * (180.0f / 3.14159265f);
    float targetCountsX = deltaYawDeg / (0.022f * sens);
    float targetCountsY = deltaPitchDeg / (0.022f * sens);

    stepX = (targetCountsX * alpha) + s_aimRemainderX;
    stepY = (targetCountsY * alpha) + s_aimRemainderY;

    // Guarantee minimum movement when targetDist >= 2.0px so steps don't freeze
    if (targetDist >= 2.0f) {
      if (std::abs(stepX) < 0.5f && targetCountsX != 0.0f)
        stepX = (targetCountsX > 0.0f) ? 0.55f : -0.55f;
      if (std::abs(stepY) < 0.5f && targetCountsY != 0.0f)
        stepY = (targetCountsY > 0.0f) ? 0.55f : -0.55f;
    }

    // Safety clamp per frame: prevent extreme single-frame jerks
    float maxStep = 32.0f * std::clamp(dt * 120.0f, 0.3f, 2.5f);
    stepX = std::clamp(stepX, -maxStep, maxStep);
    stepY = std::clamp(stepY, -maxStep, maxStep);

    long dx = (long)std::lroundf(stepX);
    long dy = (long)std::lroundf(stepY);
    s_aimRemainderX = stepX - (float)dx;
    s_aimRemainderY = stepY - (float)dy;
    if (dx || dy)
      DriverMouseMove(dx, dy);
  } else {
    s_aimRemainderX = 0.0f;
    s_aimRemainderY = 0.0f;
  }
}

// ============ TRIGGERBOT (uses FrameContext) ============
static auto s_lastTrigTime = std::chrono::steady_clock::now();
static void TriggerbotTick(const FrameContext &ctx) {
  if (!g_masterEnabled || !triggerbotEnabled || menuOpen) {
    s_lastTrigTime = std::chrono::steady_clock::now();
    return;
  }
  if (pid && ctx.foregroundPid != pid && ctx.foregroundHwnd != hwndOverlay)
    return;
  if (triggerMode == 0) {
    int tvk = (g_triggerKeyVk > 0) ? g_triggerKeyVk : VK_XBUTTON1;
    bool tHeld = ((GetAsyncKeyState(tvk) & 0x8000) != 0);
    if (!tHeld) {
      if (tvk == VK_MENU)
        tHeld = ((GetAsyncKeyState(VK_LMENU) & 0x8000) != 0) ||
                ((GetAsyncKeyState(VK_RMENU) & 0x8000) != 0);
      else if (tvk == VK_SHIFT)
        tHeld = ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0) ||
                ((GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0);
      else if (tvk == VK_CONTROL)
        tHeld = ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0) ||
                ((GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0);
      else if (tvk == VK_XBUTTON1 || tvk == 0x05)
        tHeld = ((GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0);
      else if (tvk == VK_XBUTTON2 || tvk == 0x06)
        tHeld = ((GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0);
      if (!tHeld && ((GetAsyncKeyState(VK_CAPITAL) & 0x8000) != 0))
        tHeld = true;
    }
    if (!tHeld)
      return;
  }
  if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
    s_lastTrigTime = std::chrono::steady_clock::now();
    return;
  }
  const WeaponConfig &wcfg = *ctx.wcfg;
  if (wcfg.hit_chance < 99.5f) {
    float roll = (float)(rand() % 1000) / 10.0f;
    if (roll > wcfg.hit_chance) {
      s_lastTrigTime = std::chrono::steady_clock::now();
      return;
    }
  }
  float cx = ctx.screenW / 2.0f, cy = ctx.screenH / 2.0f;
  float trigRad = (triggerRadius > 2.0f) ? triggerRadius : 22.0f;
  bool onTarget = false;
  const auto &snap = *ctx.snap;
  Vec3 fwd = {ctx.viewMatrix->m[2][0], ctx.viewMatrix->m[2][1], ctx.viewMatrix->m[2][2]};
  for (int e = 0; e < snap.count && !onTarget; e++) {
    const auto &ent = snap.entities[e];
    if (!ent.valid || ent.health <= 0)
      continue;
    if (wcfg.trigger_min_damage > 1 && ent.health < wcfg.trigger_min_damage)
      continue;
    if (teamCheck && ctx.localTeam && ent.team == ctx.localTeam)
      continue;
    if (aimbotWallCheck && !ent.spottedByLocal)
      continue;

    Vec3 toEnt = ent.headPos - ctx.localOrigin;
    float fwdDot = toEnt.x * fwd.x + toEnt.y * fwd.y + toEnt.z * fwd.z;
    if (fwdDot < 0.5f)
      continue;
    float toEntLenSq = toEnt.DistToSq({0, 0, 0});
    if (fwdDot * fwdDot < 0.5f * toEntLenSq)
      continue;

    int bonesToCheck[2] = {7, 4};
    for (int b : bonesToCheck) {
      Vec3 bw =
          (ent.hasBones && (ent.bones[b].x || ent.bones[b].y || ent.bones[b].z))
              ? ent.bones[b]
              : ent.headPos;
      if (!bw.x && !bw.y && !bw.z)
        continue;
      Vec2 bs;
      if (!WorldToScreen(bw, *ctx.viewMatrix, bs))
        continue;
      float d =
          std::sqrt((bs.x - cx) * (bs.x - cx) + (bs.y - cy) * (bs.y - cy));
      if (d < trigRad) {
        onTarget = true;
        break;
      }
    }
  }
  if (!onTarget) {
    s_lastTrigTime = std::chrono::steady_clock::now();
    return;
  }
  auto now = std::chrono::steady_clock::now();
  int elapsed = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - s_lastTrigTime)
                    .count();
  int minDelay = (triggerDelay <= 0) ? 15 : (triggerDelay + 15);
  if (elapsed < minDelay)
    return;

  // Counter-strafe / quick stop assist
  float localSpeed = std::sqrt(snap.localVelocity.x * snap.localVelocity.x + snap.localVelocity.y * snap.localVelocity.y);
  bool inAir = !(snap.localFlags & 1);
  if ((wcfg.auto_stop || g_Config.other_assists.quick_stop) && !inAir && localSpeed > 25.0f) {
    if (GetAsyncKeyState('W') & 0x8000) DriverKeyTap(0x1F); // S
    else if (GetAsyncKeyState('S') & 0x8000) DriverKeyTap(0x11); // W
    if (GetAsyncKeyState('A') & 0x8000) DriverKeyTap(0x20); // D
    else if (GetAsyncKeyState('D') & 0x8000) DriverKeyTap(0x1E); // A
  }

  s_lastTrigTime = now;
  int bursts = std::clamp(wcfg.burst_count, 1, 5);
  static std::atomic<bool> s_burstPending{false};
  if (bursts > 1 && !s_burstPending.exchange(true)) {
    std::thread([bursts] {
      for (int b = 0; b < bursts; b++) {
        DriverMouseClick();
        if (b < bursts - 1)
          Sleep(35);
      }
      s_burstPending.store(false);
    }).detach();
  } else if (bursts <= 1) {
    DriverMouseClick();
  }
  static auto s_lastTrigLog =
      std::chrono::steady_clock::now() - std::chrono::seconds(5);
  auto nowLog = std::chrono::steady_clock::now();
  if (std::chrono::duration_cast<std::chrono::milliseconds>(nowLog -
                                                            s_lastTrigLog)
          .count() >= 1500) {
    ConsoleLogger::Log(
        ConsoleLogger::LOG_COMBAT,
        "Triggerbot shot dispatched (Delay: %dms, Window: %.1fpx, Bursts: %d)",
        minDelay, trigRad, bursts);
    s_lastTrigLog = nowLog;
  }
}

// ============ DX11 SETUP ============
bool CreateDeviceD3D(HWND hWnd) {
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate = {0, 1};
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  UINT flags = 0;
  D3D_FEATURE_LEVEL fl;
  const D3D_FEATURE_LEVEL fla[2] = {D3D_FEATURE_LEVEL_11_0,
                                    D3D_FEATURE_LEVEL_10_0};
  HRESULT res = D3D11CreateDeviceAndSwapChain(
      NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, fla, 2, D3D11_SDK_VERSION,
      &sd, &g_pSwapChain, &g_pd3dDevice, &fl, &g_pd3dDeviceContext);
  if (res == DXGI_ERROR_UNSUPPORTED)
    res = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_WARP, NULL, flags, fla, 2, D3D11_SDK_VERSION, &sd,
        &g_pSwapChain, &g_pd3dDevice, &fl, &g_pd3dDeviceContext);
  if (res != S_OK)
    return false;
  ID3D11Texture2D *pBB;
  g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBB));
  g_pd3dDevice->CreateRenderTargetView(pBB, NULL, &g_mainRenderTargetView);
  pBB->Release();
  D3D11_BLEND_DESC bd = {};
  bd.RenderTarget[0].BlendEnable = TRUE;
  bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
  bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
  float bf[4] = {};
  g_pd3dDeviceContext->OMSetBlendState(g_pBlendState, bf, 0xffffffff);
  return true;
}
void CleanupDeviceD3D() {
  if (g_pBlendState) {
    g_pBlendState->Release();
    g_pBlendState = NULL;
  }
  if (g_mainRenderTargetView) {
    g_mainRenderTargetView->Release();
    g_mainRenderTargetView = NULL;
  }
  if (g_pSwapChain) {
    g_pSwapChain->Release();
    g_pSwapChain = NULL;
  }
  if (g_pd3dDeviceContext) {
    g_pd3dDeviceContext->Release();
    g_pd3dDeviceContext = NULL;
  }
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = NULL;
  }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;
  switch (msg) {
  case WM_SIZE:
    if (g_pd3dDevice && wParam != SIZE_MINIMIZED) {
      if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
      }
      g_pSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam),
                                  DXGI_FORMAT_UNKNOWN, 0);
      ID3D11Texture2D *pBB;
      g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBB));
      g_pd3dDevice->CreateRenderTargetView(pBB, NULL, &g_mainRenderTargetView);
      pBB->Release();
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU)
      return 0;
    break;
  case WM_NCHITTEST: {
    if (!menuOpen && !g_Config.island.enabled)
      return HTTRANSPARENT;
    ImGuiContext *g = ImGui::GetCurrentContext();
    if (g) {
      POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
      ScreenToClient(hWnd, &pt);
      ImVec2 mp((float)pt.x, (float)pt.y);
      if (g->MovingWindow || ImGui::GetIO().MouseDown[0] ||
          ImGui::GetIO().MouseDown[1])
        return HTCLIENT;
      if (g->OpenPopupStack.Size > 0)
        return HTCLIENT;
      const char *winNames[] = {"##LianFlowMainWindow", "##MainWindow", "##DynamicIsland",
                                "##SkeletonSideGui", "##HudElementSideGui"};
      for (const char *name : winNames) {
        ImGuiWindow *win = ImGui::FindWindowByName(name);
        if (win && win->Active && !win->Hidden) {
          ImRect bb(win->Pos,
                    ImVec2(win->Pos.x + win->Size.x, win->Pos.y + win->Size.y));
          bb.Expand(14.0f);
          if (bb.Contains(mp))
            return HTCLIENT;
        }
      }
    }
    return HTTRANSPARENT;
  }
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  }
  return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// ============ WINMAIN ============
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
  DWORD currentPid = GetCurrentProcessId();
  UniqueHandle hSnapP(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
  if (hSnapP) {
    PROCESSENTRY32W pe = {sizeof(pe)};
    if (Process32FirstW(hSnapP.get(), &pe)) {
      do {
        if (pe.th32ProcessID != currentPid &&
            (_wcsicmp(pe.szExeFile, L"esp_fast.exe") == 0 ||
             _wcsicmp(pe.szExeFile, L"esp.exe") == 0)) {
          UniqueHandle hOld(
              OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
                          pe.th32ProcessID));
          if (hOld) {
            ConsoleLogger::Init();
            ConsoleLogger::Log(
                ConsoleLogger::LOG_WARN,
                "Another overlay instance detected (pid %lu). Not killing it.",
                (unsigned long)pe.th32ProcessID);
          }
        }
      } while (Process32NextW(hSnapP.get(), &pe));
    }
  }

  bool skipConsole =
      (strstr(GetCommandLineA(), "--no-driver-console") != nullptr);
  if (!skipConsole) {
    AllocConsole();
    FILE *fpDummy;
    freopen_s(&fpDummy, "CONOUT$", "w", stdout);
    freopen_s(&fpDummy, "CONOUT$", "w", stderr);
    freopen_s(&fpDummy, "CONIN$", "r", stdin);
    HWND hConsole = GetConsoleWindow();
    if (hConsole) {
      HMENU hMenu = GetSystemMenu(hConsole, FALSE);
      if (hMenu)
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
      SetConsoleTitleA(
          "Zolo's CS2 External — Live Diagnostic & Offset Terminal");
    }
    ConsoleLogger::Init();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n");
    printf("  ============================================================\n");
    printf("    ZOLO'S CS2 EXTERNAL  --  LIVE DIAGNOSTIC & OFFSET TERMINAL \n");
    printf(
        "  ============================================================\n\n");
    wchar_t exePath[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    wchar_t *lastSlash = wcsrchr(exePath, L'\\');
    if (lastSlash)
      *(lastSlash + 1) = L'\0';
    wchar_t driverPath[MAX_PATH] = {0};
    swprintf_s(driverPath, L"%s204.sys", exePath);
    ConsoleLogger::Log(ConsoleLogger::LOG_INFO, "Target Driver: %ls",
                       driverPath);
    ConsoleLogger::Log(
        ConsoleLogger::LOG_INFO,
        "Connecting to Service [MatchIo] / Device [\\\\.\\MatchIo]...");
    if (GetFileAttributesW(driverPath) == INVALID_FILE_ATTRIBUTES) {
      ConsoleLogger::Log(ConsoleLogger::LOG_WARN,
                         "Warning: 204.sys not found in local directory!");
    }
    bool driverOk = matchio::g_InputDevice.init(driverPath);
    if (driverOk) {
      ConsoleLogger::Log(ConsoleLogger::LOG_SUCCESS,
                         "Kernel driver loaded and connected successfully!");
      ConsoleLogger::Log(
          ConsoleLogger::LOG_SUCCESS,
          "Hardware-level mouse/keyboard input dispatch: ACTIVE");
      g_Config.driver_active = true;
      snprintf(g_Config.driver_status_text, sizeof(g_Config.driver_status_text),
               "Connected (MatchIo)");
    } else {
      ConsoleLogger::Log(
          ConsoleLogger::LOG_INFO,
          "Usermode input dispatch: ACTIVE (mouse_event / SendInput)");
      g_Config.driver_active = false;
      snprintf(g_Config.driver_status_text, sizeof(g_Config.driver_status_text),
               "Usermode (SendInput)");
    }
    ConsoleLogger::Log(ConsoleLogger::LOG_INFO,
                       "Diagnostic terminal active (Console Close 'X' disabled "
                       "to prevent crash).");
    ConsoleLogger::Log(
        ConsoleLogger::LOG_INFO,
        "DirectX 11 overlay and ImGui interface initializing...");
  } else {
    wchar_t exePath[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    wchar_t *lastSlash = wcsrchr(exePath, L'\\');
    if (lastSlash)
      *(lastSlash + 1) = L'\0';
    wchar_t driverPath[MAX_PATH] = {0};
    swprintf_s(driverPath, L"%s204.sys", exePath);
    g_Config.driver_active = matchio::g_InputDevice.init(driverPath);
    if (g_Config.driver_active) {
      snprintf(g_Config.driver_status_text, sizeof(g_Config.driver_status_text),
               "Connected (MatchIo)");
    } else {
      snprintf(g_Config.driver_status_text, sizeof(g_Config.driver_status_text),
               "Usermode (SendInput)");
    }
  }

  TimerGuard _timer;
  InitializeCriticalSection(&g_snapLock);

  int posX = 0, posY = 0;
  char obf_sdl[10] = {};
  OBF_DEC(obf_sdl, "SDL_app");
  char obf_cs2[18] = {};
  OBF_DEC(obf_cs2, "Counter-Strike 2");
  char obf_cdll[12] = {};
  OBF_DEC(obf_cdll, "client.dll");
  g_hwndCS2 = FindWindowA(obf_sdl, obf_cs2);
  if (!g_hwndCS2)
    g_hwndCS2 = FindWindowA(nullptr, obf_cs2);
  if (g_hwndCS2) {
    GetWindowThreadProcessId(g_hwndCS2, &pid);
    RECT rect;
    GetClientRect(g_hwndCS2, &rect);
    if (rect.right - rect.left > 0)
      screenWidth = rect.right - rect.left;
    if (rect.bottom - rect.top > 0)
      screenHeight = rect.bottom - rect.top;
    POINT pt = {0, 0};
    ClientToScreen(g_hwndCS2, &pt);
    posX = pt.x;
    posY = pt.y;
  }

  const wchar_t *spoof_cls = stealth::spoof_class_name();
  const wchar_t *spoof_title = stealth::spoof_window_title();
  WNDCLASSEXW wc = {sizeof(wc), CS_CLASSDC, WndProc,   0L,
                    0L,         hInstance,  nullptr,   nullptr,
                    nullptr,    nullptr,    spoof_cls, nullptr};
  ::RegisterClassExW(&wc);
  hwndOverlay = ::CreateWindowExW(
      WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, spoof_cls, spoof_title,
      WS_POPUP, posX, posY, screenWidth, screenHeight, nullptr, nullptr,
      wc.hInstance, nullptr);
  SetLayeredWindowAttributes(hwndOverlay, 0, 255, LWA_ALPHA);
  MARGINS margins = {-1, -1, -1, -1};
  DwmExtendFrameIntoClientArea(hwndOverlay, &margins);
  if (!CreateDeviceD3D(hwndOverlay)) {
    CleanupDeviceD3D();
    ::UnregisterClassW(spoof_cls, wc.hInstance);
    return 1;
  }
  ::ShowWindow(hwndOverlay, SW_SHOWNOACTIVATE);
  ::UpdateWindow(hwndOverlay);
  stealth::init(matchio::g_InputDevice.native_handle(), hwndOverlay);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.IniFilename = NULL;
  io.ConfigWindowsResizeFromEdges = true;
  ImGuiStyle &style = ImGui::GetStyle();
  style.TouchExtraPadding = ImVec2(4.0f, 4.0f);
  style.AntiAliasedLines = false;
  style.AntiAliasedFill = false;
  ImGui_ImplWin32_Init(hwndOverlay);
  ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

  Inter_S = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsSemiBold,
                                           sizeof(PoppinsSemiBold), 17.f, NULL,
                                           io.Fonts->GetGlyphRangesCyrillic());
  Inter_S_3 = io.Fonts->AddFontFromMemoryTTF(
      (void*)PoppinsSemiBold, sizeof(PoppinsSemiBold), 15.f, NULL,
      io.Fonts->GetGlyphRangesCyrillic());
  Icon =
      io.Fonts->AddFontFromMemoryTTF((void*)PoppinsBold, sizeof(PoppinsBold), 26.f, NULL,
                                     io.Fonts->GetGlyphRangesCyrillic());

  static ImWchar icomoon_ranges[] = { 0x1, 0xFFFF, 0 };
  static ImFontConfig icomoon_config;
  icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
  icomoon_config.MergeMode = true;
  icomoon_config.GlyphOffset.y = 2;

  ImFontConfig lianCfg;
  font::esp_font = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsRegular, sizeof(PoppinsRegular), 17.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());
  font::regular_m = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsMedium, sizeof(PoppinsMedium), 21.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());
  font::regular_l = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsMedium, sizeof(PoppinsMedium), 41.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());
  font::s_inter_semibold = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsSemiBold, sizeof(PoppinsSemiBold), 17.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());
  font::bold_font = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsBold, sizeof(PoppinsBold), 23.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());
  io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 32.f, &icomoon_config, icomoon_ranges);
  font::inter_medium = io.Fonts->AddFontFromMemoryTTF((void*)PoppinsMedium, sizeof(PoppinsMedium), 17.f, &lianCfg, io.Fonts->GetGlyphRangesCyrillic());

  static const ImWchar gui_glyph_ranges[] = {0x0020, 0x00FF, 0x2000, 0x206F, 0};
  const char *font_regular = "C:\\Windows\\Fonts\\segoeui.ttf";
  const char *font_semibold = "C:\\Windows\\Fonts\\seguisb.ttf";
  const char *font_bold_ng = "C:\\Windows\\Fonts\\segoeuib.ttf";
  const char *font_light = "C:\\Windows\\Fonts\\segoeuil.ttf";
  if (GetFileAttributesA(font_regular) != INVALID_FILE_ATTRIBUTES) {
    g_FontRegular = io.Fonts->AddFontFromFileTTF(font_regular, 13.5f, nullptr,
                                                 gui_glyph_ranges);
    g_FontSubNav = io.Fonts->AddFontFromFileTTF(font_regular, 13.0f, nullptr,
                                                gui_glyph_ranges);
    g_FontBrandVer = io.Fonts->AddFontFromFileTTF(font_regular, 10.5f, nullptr,
                                                  gui_glyph_ranges);
    g_FontSmall = io.Fonts->AddFontFromFileTTF(font_regular, 11.5f, nullptr,
                                               gui_glyph_ranges);
    g_FontNav = g_FontRegular;
  }
  const char *sb_path =
      (GetFileAttributesA(font_semibold) != INVALID_FILE_ATTRIBUTES)
          ? font_semibold
          : font_bold_ng;
  if (GetFileAttributesA(sb_path) != INVALID_FILE_ATTRIBUTES) {
    g_FontSemibold =
        io.Fonts->AddFontFromFileTTF(sb_path, 13.5f, nullptr, gui_glyph_ranges);
    g_FontSubNavActive =
        io.Fonts->AddFontFromFileTTF(sb_path, 13.0f, nullptr, gui_glyph_ranges);
    g_FontSmallBold =
        io.Fonts->AddFontFromFileTTF(sb_path, 11.5f, nullptr, gui_glyph_ranges);
    g_FontNavActive = g_FontSemibold;
  }
  if (GetFileAttributesA(font_bold_ng) != INVALID_FILE_ATTRIBUTES) {
    g_FontBrandTitle = io.Fonts->AddFontFromFileTTF(font_bold_ng, 17.5f,
                                                    nullptr, gui_glyph_ranges);
    g_FontBold = io.Fonts->AddFontFromFileTTF(font_bold_ng, 14.5f, nullptr,
                                              gui_glyph_ranges);
    g_FontSection = io.Fonts->AddFontFromFileTTF(font_bold_ng, 9.5f, nullptr,
                                                 gui_glyph_ranges);
    g_FontHeader = g_FontBold;
  }
  if (GetFileAttributesA(font_light) != INVALID_FILE_ATTRIBUTES) {
    g_FontLight = io.Fonts->AddFontFromFileTTF(font_light, 13.0f, nullptr,
                                               gui_glyph_ranges);
  }
  for (int i = 0; i < 6; i++)
    g_EspFonts[i] = g_FontRegular;

  Theme::SetupImGuiStyle(g_Config.is_dark_mode);
  HelloIntro::Init(g_pd3dDevice);
  ReactiveState::InitializeDefaultSubscribers();

  if (autoLoadConfig) {
    if (!LoadConfig("config.cfg"))
      SaveConfig("config.cfg");
    SyncConfigToGlobals();
  } else {
    SyncConfigToGlobals();
  }

  std::thread memThread(MemoryWorkerThread);
  std::thread slowThread(SlowScanThread);

  bool lastInsertDown = false, lastHomeDown = false;
  auto lastFrameTime = std::chrono::high_resolution_clock::now();
  int frameCounter = 0;
  float fpsTimer = 0.0f;
  SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

  DEVMODEA dm = {};
  dm.dmSize = sizeof(dm);
  if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm) &&
      dm.dmDisplayFrequency > 60) {
    if (g_Config.max_fps == 240 && dm.dmDisplayFrequency > 240)
      g_Config.max_fps = (int)dm.dmDisplayFrequency;
  }
  auto nextFrameTime = std::chrono::high_resolution_clock::now();

  MSG msg;
  ZeroMemory(&msg, sizeof(msg));
  while (running) {
    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT) {
        running = false;
        break;
      }
    }
    if (!running)
      break;

    static int winCheckTicks = 0;
    if (++winCheckTicks >= 60 || !g_hwndCS2 || !IsWindow(g_hwndCS2)) {
      winCheckTicks = 0;
      if (!g_hwndCS2 || !IsWindow(g_hwndCS2)) {
        static char s_obf_sdl[16] = {};
        static char s_obf_cs2[24] = {};
        static bool s_obf_inited = false;
        if (!s_obf_inited) {
          OBF_DEC(s_obf_sdl, "SDL_app");
          OBF_DEC(s_obf_cs2, "Counter-Strike 2");
          s_obf_inited = true;
        }
        g_hwndCS2 = FindWindowA(s_obf_sdl, s_obf_cs2);
        if (!g_hwndCS2)
          g_hwndCS2 = FindWindowA(NULL, s_obf_cs2);
        if (g_hwndCS2 && !pid)
          GetWindowThreadProcessId(g_hwndCS2, &pid);
      }
      if (g_hwndCS2 && IsWindow(g_hwndCS2)) {
        RECT r;
        if (GetClientRect(g_hwndCS2, &r)) {
          int w = r.right - r.left, h = r.bottom - r.top;
          POINT pt = {0, 0};
          ClientToScreen(g_hwndCS2, &pt);
          static int lx = -9999, ly = -9999;
          if (w > 0 && h > 0 &&
              (w != screenWidth || h != screenHeight || pt.x != lx ||
               pt.y != ly)) {
            screenWidth = w;
            screenHeight = h;
            lx = pt.x;
            ly = pt.y;
            SetWindowPos(hwndOverlay, HWND_TOPMOST, pt.x, pt.y, w, h,
                         SWP_NOACTIVATE | SWP_SHOWWINDOW);
          }
        }
      }
    }

    bool insertDown = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
    if (insertDown && !lastInsertDown) {
      menuOpen = !menuOpen;
      io.MouseDrawCursor = menuOpen;
    }
    lastInsertDown = insertDown;
    bool homeDown = (GetAsyncKeyState(VK_OEM_6) & 0x8000) != 0;
    if (homeDown && !lastHomeDown)
      g_masterEnabled = !g_masterEnabled;
    lastHomeDown = homeDown;

    static bool s_overlayInteractive = false;
    bool shouldBeInteractive = menuOpen || g_Config.island.enabled;
    if (shouldBeInteractive) {
      ImGuiContext *g = ImGui::GetCurrentContext();
      if (g && (g->MovingWindow || io.MouseDown[0] || io.MouseDown[1] || g->OpenPopupStack.Size > 0)) {
        shouldBeInteractive = true;
      }
    }
    if (s_overlayInteractive != shouldBeInteractive) {
      s_overlayInteractive = shouldBeInteractive;
      LONG_PTR ex = GetWindowLongPtrA(hwndOverlay, GWL_EXSTYLE);
      SetWindowLongPtrA(hwndOverlay, GWL_EXSTYLE,
                        shouldBeInteractive ? (ex & ~WS_EX_TRANSPARENT)
                                            : (ex | WS_EX_TRANSPARENT));
    }

    if (g_hwndCS2 && IsIconic(g_hwndCS2)) {
      Sleep(50);
      continue;
    }

    auto cur = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(cur - lastFrameTime).count();
    lastFrameTime = cur;
    if ((fpsTimer += dt) >= 1.0f) {
      telemetryFps.store(++frameCounter);
      frameCounter = 0;
      fpsTimer = 0.f;
    } else
      ++frameCounter;

    // ---- Frame context build (unified matrix + cached weapon config) ----
    int readSlot = g_snapWriteIdx.load(std::memory_order_acquire);
    g_snapRenderReading.store(readSlot, std::memory_order_release);
    const FrameSnapshot &snap = g_snapBuf[readSlot];

    Matrix4x4 liveVM = snap.viewMatrix;
    bool needLiveMatrix = g_masterEnabled && (espEnabled || aimbotEnabled || triggerbotEnabled || !menuOpen);
    if (needLiveMatrix && hGame && clientBase) {
      ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwViewMatrix), &liveVM,
                        sizeof(liveVM), nullptr);
      UpdateW2SCache(liveVM, (float)screenWidth, (float)screenHeight, g_w2sCache);
    }

    // Cache active weapon config once per frame
    int activeWpnIdx =
        (g_Config.active_weapon >= 0 && g_Config.active_weapon < WEAPON_COUNT)
            ? g_Config.active_weapon
            : WEAPON_GENERAL;
    const WeaponConfig &curWcfg = g_Config.weapons[activeWpnIdx];
    const WeaponConfig &activeWcfg =
        (curWcfg.enabled || activeWpnIdx == WEAPON_GENERAL)
            ? curWcfg
            : (g_Config.weapons[WEAPON_GENERAL].enabled
                   ? g_Config.weapons[WEAPON_GENERAL]
                   : curWcfg);

    static HWND s_fgHwnd = nullptr;
    static DWORD s_fgPid = 0;
    static uint64_t s_fgRefreshMs = 0;
    if (!menuOpen) {
      uint64_t nowMsFg = GetTickCount64();
      if (nowMsFg - s_fgRefreshMs >= 500) {
        s_fgRefreshMs = nowMsFg;
        s_fgHwnd = GetForegroundWindow();
        s_fgPid = 0;
        if (s_fgHwnd)
          GetWindowThreadProcessId(s_fgHwnd, &s_fgPid);
      }
    }

    FrameContext ctx{};
    ctx.snap = &snap;
    ctx.viewMatrix = &liveVM;
    ctx.wcfg = &activeWcfg;
    ctx.localOrigin = snap.localOrigin;
    ctx.localTeam = snap.localTeam;
    ctx.screenW = screenWidth;
    ctx.screenH = screenHeight;
    ctx.foregroundHwnd = s_fgHwnd;
    ctx.foregroundPid = s_fgPid;

    const Vec3 &localOrigin = snap.localOrigin;
    const int localTeam = snap.localTeam;

    int sW = g_slowRing.publishedIdx.load(std::memory_order_acquire);
    const SlowSnapshot &slowLocal = g_slowRing.data[sW];

    if (g_Config.auto_weapon_detect && snap.localWeaponDefIdx > 0) {
      int autoId = WeaponIDFromDefIdx(snap.localWeaponDefIdx);
      if (autoId >= 0 && autoId < WEAPON_COUNT &&
          g_Config.active_weapon != autoId) {
        g_Config.active_weapon = autoId;
        SyncConfigToGlobals();
        ConsoleLogger::Log(
            ConsoleLogger::LOG_INFO,
            "Equipped weapon changed: '%s' -> Auto-selected profile [%s]",
            snap.localWeaponName[0] ? snap.localWeaponName : "Weapon",
            kWeaponProfileNames[autoId]);
      }
    }

    static bool s_lastMenuOpenState = false;
    bool wasOpen = (menuOpen != s_lastMenuOpenState);
    s_lastMenuOpenState = menuOpen;
    if (wasOpen || g_config_dirty.exchange(false))
      SyncConfigToGlobals();

    if (g_masterEnabled && !menuOpen) {
      AimbotTick(ctx);
      TriggerbotTick(ctx);
    }

    // Auto Pistol
    if (g_masterEnabled && !menuOpen && g_Config.other_assists.auto_pistol) {
      int curWpnCat = WeaponIDFromDefIdx(snap.localWeaponDefIdx);
      if (curWpnCat == WEAPON_PISTOL || curWpnCat == WEAPON_SILENCED_PISTOL) {
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
          static auto s_lastAutoPistol = std::chrono::steady_clock::now();
          auto apNow = std::chrono::steady_clock::now();
          if (std::chrono::duration_cast<std::chrono::milliseconds>(apNow - s_lastAutoPistol).count() >= 65) {
            DriverMouseClick();
            s_lastAutoPistol = apNow;
          }
        }
      }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();

    if (!g_masterEnabled) {
      const char *txt = "[ZOLO'S EXTERNAL — PAUSED (']' to resume)]";
      ImVec2 tsz = ImGui::CalcTextSize(txt);
      drawList->AddText({screenWidth / 2.0f - tsz.x / 2.0f + 1, 11.f},
                        IM_COL32(0, 0, 0, 220), txt);
      drawList->AddText({screenWidth / 2.0f - tsz.x / 2.0f, 10.f},
                        IM_COL32(255, 80, 80, 255), txt);
    }

    if (!menuOpen && !g_Config.watermark.enabled) {
      char statusBuf[160];
      sprintf(statusBuf,
              "SPRING EXTERNAL  |  %d TGT  |  %d FPS  |  %s  [INSERT: MENU | "
              "]: PANIC]",
              telemetryTargets.load(), telemetryFps.load(),
              g_masterEnabled ? "ON" : "OFF");
      drawList->AddText({16, 16}, IM_COL32(0, 0, 0, 220), statusBuf);
      drawList->AddText({15, 15}, IM_COL32(195, 120, 255, 230), statusBuf);
    }

    // ==== ESP RENDER PASS ====
    if (espEnabled && g_masterEnabled && snap.count > 0) {
      const ImU32 kBoxOutl = IM_COL32(0, 0, 0, 220);
      const ImU32 kBarOutl = IM_COL32(0, 0, 0, 220);
      const bool kVisCheck = g_Config.visuals.visible_check;
      const ImU32 kCfgBoxCol =
          IM_COL32((uint8_t)(g_Config.visuals.box_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_color[3] * 255.f));
      const ImU32 kCfgBoxFill =
          IM_COL32((uint8_t)(g_Config.visuals.box_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_fill_opacity * 255.f));
      const ImU32 kCfgBoxColVis =
          IM_COL32((uint8_t)(g_Config.visuals.box_visible_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_visible_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_visible_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_visible_color[3] * 255.f));
      const ImU32 kCfgBoxFillVis =
          IM_COL32((uint8_t)(g_Config.visuals.box_visible_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_visible_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_visible_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_fill_opacity * 255.f));
      const ImU32 kCfgBoxColOccl =
          IM_COL32((uint8_t)(g_Config.visuals.box_occluded_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_occluded_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_occluded_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_occluded_color[3] * 255.f));
      const ImU32 kCfgBoxFillOccl =
          IM_COL32((uint8_t)(g_Config.visuals.box_occluded_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.box_occluded_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.box_occluded_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.box_fill_opacity * 255.f));
      const ImU32 kCfgNameCol =
          IM_COL32((uint8_t)(g_Config.visuals.name_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.name_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.name_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.name_color[3] * 255.f));
      const ImU32 kCfgWpnCol =
          IM_COL32((uint8_t)(g_Config.visuals.weapon_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.weapon_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.weapon_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.weapon_color[3] * 255.f));
      const ImU32 kCfgSkelCol0 = (uint8_t)(g_Config.skeleton.color[0] * 255.f);
      const ImU32 kCfgSkelCol1 = (uint8_t)(g_Config.skeleton.color[1] * 255.f);
      const ImU32 kCfgSkelCol2 = (uint8_t)(g_Config.skeleton.color[2] * 255.f);
      const ImU32 kCfgSkelCol3 = (uint8_t)(g_Config.skeleton.color[3] * 255.f);
      const ImU32 kCfgSkelOccl0 =
          (uint8_t)(g_Config.skeleton.occluded_color[0] * 255.f);
      const ImU32 kCfgSkelOccl1 =
          (uint8_t)(g_Config.skeleton.occluded_color[1] * 255.f);
      const ImU32 kCfgSkelOccl2 =
          (uint8_t)(g_Config.skeleton.occluded_color[2] * 255.f);
      const ImU32 kCfgSkelOccl3 =
          (uint8_t)(g_Config.skeleton.occluded_color[3] * 255.f);
      const float kBThick = g_Config.visuals.box_thickness > 0.5f
                                ? g_Config.visuals.box_thickness
                                : 1.8f;
      const float kSkThick = g_Config.skeleton.thickness > 0.5f
                                 ? g_Config.skeleton.thickness
                                 : 1.5f;
      const float kHpBW = g_Config.visuals.hp_bar_width > 1.0f
                              ? g_Config.visuals.hp_bar_width
                              : 3.5f;
      const ImU32 kCfgHeadCol =
          IM_COL32((uint8_t)(g_Config.visuals.head_pos_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.head_pos_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.head_pos_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.head_pos_color[3] * 255.f));
      const ImU32 kCfgSnapCol =
          IM_COL32((uint8_t)(g_Config.visuals.snaplines_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.snaplines_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.snaplines_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.snaplines_color[3] * 255.f));
      const ImU32 kCfgEyeCol =
          IM_COL32((uint8_t)(g_Config.visuals.eye_ray_color[0] * 255.f),
                   (uint8_t)(g_Config.visuals.eye_ray_color[1] * 255.f),
                   (uint8_t)(g_Config.visuals.eye_ray_color[2] * 255.f),
                   (uint8_t)(g_Config.visuals.eye_ray_color[3] * 255.f));
      const ImU32 kCfgBombBorder = IM_COL32(
          (uint8_t)(g_Config.visuals.bomb_carrier_color[0] * 255.f),
          (uint8_t)(g_Config.visuals.bomb_carrier_color[1] * 255.f),
          (uint8_t)(g_Config.visuals.bomb_carrier_color[2] * 255.f), 255);
      const int kCfgHpPos = g_Config.visuals.esp_hp_pos;
      const bool kCfgHpGrad = g_Config.visuals.hp_gradient;
      const ImU32 kCfgHpSolidCol = IM_COL32((uint8_t)(g_Config.visuals.hp_color[0] * 255.f),
                                           (uint8_t)(g_Config.visuals.hp_color[1] * 255.f),
                                           (uint8_t)(g_Config.visuals.hp_color[2] * 255.f), 255);
      const int kCfgHealthType = g_Config.visuals.health_type;
      const bool kCfgOptBoxFill = g_Config.visuals.box_fill;
      const bool kCfgOptBoxOutline = g_Config.visuals.box_outline;

      float dtSnap = std::chrono::duration<float>(cur - snap.timestamp).count();
      dtSnap = std::clamp(dtSnap, 0.0f, 0.033f);
      constexpr float kTextLodDistSq = (150.0f / 0.0254f) * (150.0f / 0.0254f);

      static ImVec2 s_c4TagSize = {0, 0};
      static bool s_c4TagSized = false;
      if (!s_c4TagSized) {
        s_c4TagSize = ImGui::CalcTextSize("[BOMB CARRIER]");
        s_c4TagSized = true;
      }

      for (int e = 0; e < snap.count; e++) {
        const auto &ent = snap.entities[e];
        if (!ent.valid)
          continue;
        bool isTeammate = (localTeam && ent.team == localTeam);
        if (g_Config.visuals.enemies && isTeammate)
          continue;

        bool hasVel = (ent.velocity.x != 0.0f || ent.velocity.y != 0.0f ||
                       ent.velocity.z != 0.0f);
        Vec3 entOrigin =
            hasVel ? ent.origin + (ent.velocity * dtSnap) : ent.origin;
        Vec3 entHead =
            hasVel ? ent.headPos + (ent.velocity * dtSnap) : ent.headPos;
        Vec2 headScr, originScr;
        if (!WorldToScreen(entHead, liveVM, headScr) ||
            !WorldToScreen(entOrigin, liveVM, originScr))
          continue;
        float boxH = (originScr.y - headScr.y) * 1.09f;
        if (boxH < 5.0f)
          boxH = 5.0f;
        float boxW = boxH * 0.60f;
        float boxTop = headScr.y - (boxH * 0.08f);
        float boxLeft = originScr.x - (boxW / 2.0f);
        float boxRight = boxLeft + boxW;
        float boxBtm = boxTop + boxH;
        if (boxRight < -80.0f || boxLeft > (float)screenWidth + 80.0f ||
            boxBtm < -80.0f || boxTop > (float)screenHeight + 80.0f)
          continue;

        const float distSq = localOrigin.DistToSq(ent.origin);
        const float distU = sqrtf(distSq);
        const float distM = distU * 0.0254f;
        const bool farLOD = (distSq > kTextLodDistSq);

        ImU32 curBoxColor, curBoxFill;
        if (isTeammate) {
          curBoxColor = IM_COL32(0, 220, 140, 255);
          curBoxFill =
              IM_COL32(0, 220, 140,
                       (uint8_t)(g_Config.visuals.box_fill_opacity * 255.f));
        } else if (kVisCheck) {
          curBoxColor = ent.spotted ? kCfgBoxColVis : kCfgBoxColOccl;
          curBoxFill = ent.spotted ? kCfgBoxFillVis : kCfgBoxFillOccl;
        } else {
          curBoxColor = kCfgBoxCol;
          curBoxFill = kCfgBoxFill;
        }

        // 1. Box ESP
        if (drawBoxes) {
          if (boxStyle == 0) {
            if (g_Config.visuals.box_fill)
              drawList->AddRectFilled({boxLeft, boxTop}, {boxRight, boxBtm},
                                      curBoxFill);
            float cL =
                std::min(boxW * 0.45f, g_Config.visuals.box_arm_length > 1.0f
                                           ? g_Config.visuals.box_arm_length
                                           : boxW * 0.25f);
            if (g_Config.visuals.box_outline) {
              drawList->AddLine({boxLeft - 1, boxTop},
                                {boxLeft + cL + 1, boxTop}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxLeft, boxTop - 1},
                                {boxLeft, boxTop + cL + 1}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxRight - cL - 1, boxTop},
                                {boxRight + 1, boxTop}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxRight, boxTop - 1},
                                {boxRight, boxTop + cL + 1}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxLeft - 1, boxBtm},
                                {boxLeft + cL + 1, boxBtm}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxLeft, boxBtm - cL - 1},
                                {boxLeft, boxBtm + 1}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxRight - cL - 1, boxBtm},
                                {boxRight + 1, boxBtm}, kBoxOutl,
                                kBThick + 1.6f);
              drawList->AddLine({boxRight, boxBtm - cL - 1},
                                {boxRight, boxBtm + 1}, kBoxOutl,
                                kBThick + 1.6f);
            }
            drawList->AddLine({boxLeft, boxTop}, {boxLeft + cL, boxTop},
                              curBoxColor, kBThick);
            drawList->AddLine({boxLeft, boxTop}, {boxLeft, boxTop + cL},
                              curBoxColor, kBThick);
            drawList->AddLine({boxRight - cL, boxTop}, {boxRight, boxTop},
                              curBoxColor, kBThick);
            drawList->AddLine({boxRight, boxTop}, {boxRight, boxTop + cL},
                              curBoxColor, kBThick);
            drawList->AddLine({boxLeft, boxBtm}, {boxLeft + cL, boxBtm},
                              curBoxColor, kBThick);
            drawList->AddLine({boxLeft, boxBtm - cL}, {boxLeft, boxBtm},
                              curBoxColor, kBThick);
            drawList->AddLine({boxRight - cL, boxBtm}, {boxRight, boxBtm},
                              curBoxColor, kBThick);
            drawList->AddLine({boxRight, boxBtm - cL}, {boxRight, boxBtm},
                              curBoxColor, kBThick);
          } else if (boxStyle == 2) {
            // OPT: 3D box now uses BatchW2S_4Way (8× faster corner projection)
            float halfW = 18.0f;
            float btmZ = entOrigin.z;
            float topZ = entHead.z + 8.0f;
            float rx = liveVM.m[0][0] * halfW, ry = liveVM.m[0][1] * halfW;
            float fx = liveVM.m[2][0] * halfW, fy = liveVM.m[2][1] * halfW;
            Vec3 cornersW[8] = {
                {entOrigin.x + rx + fx, entOrigin.y + ry + fy, btmZ},
                {entOrigin.x - rx + fx, entOrigin.y - ry + fy, btmZ},
                {entOrigin.x - rx - fx, entOrigin.y - ry - fy, btmZ},
                {entOrigin.x + rx - fx, entOrigin.y + ry - fy, btmZ},
                {entOrigin.x + rx + fx, entOrigin.y + ry + fy, topZ},
                {entOrigin.x - rx + fx, entOrigin.y - ry + fy, topZ},
                {entOrigin.x - rx - fx, entOrigin.y - ry - fy, topZ},
                {entOrigin.x + rx - fx, entOrigin.y + ry - fy, topZ}};
            Vec2 cornersS[8];
            bool okA[4], okB[4];
            BatchW2S_4Way(&cornersW[0], g_w2sCache, &cornersS[0], okA);
            BatchW2S_4Way(&cornersW[4], g_w2sCache, &cornersS[4], okB);
            bool allValid = okA[0] & okA[1] & okA[2] & okA[3] & okB[0] &
                            okB[1] & okB[2] & okB[3];
            if (allValid) {
              static constexpr int edges[12][2] = {
                  {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
                  {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
              if (g_Config.visuals.box_outline) {
                for (int edg = 0; edg < 12; edg++)
                  drawList->AddLine(
                      {cornersS[edges[edg][0]].x, cornersS[edges[edg][0]].y},
                      {cornersS[edges[edg][1]].x, cornersS[edges[edg][1]].y},
                      kBoxOutl, kBThick + 1.6f);
              }
              for (int edg = 0; edg < 12; edg++)
                drawList->AddLine(
                    {cornersS[edges[edg][0]].x, cornersS[edges[edg][0]].y},
                    {cornersS[edges[edg][1]].x, cornersS[edges[edg][1]].y},
                    curBoxColor, kBThick);
              if (kCfgOptBoxFill)
                drawList->AddQuadFilled({cornersS[4].x, cornersS[4].y},
                                        {cornersS[5].x, cornersS[5].y},
                                        {cornersS[6].x, cornersS[6].y},
                                        {cornersS[7].x, cornersS[7].y},
                                        curBoxFill);
            } else {
              if (kCfgOptBoxFill)
                drawList->AddRectFilled({boxLeft, boxTop}, {boxRight, boxBtm},
                                        curBoxFill);
              if (kCfgOptBoxOutline)
                drawList->AddRect({boxLeft - 1, boxTop - 1},
                                  {boxRight + 1, boxBtm + 1}, kBoxOutl, 0, 0,
                                  kBThick + 1.6f);
              drawList->AddRect({boxLeft, boxTop}, {boxRight, boxBtm},
                                curBoxColor, 0, 0, kBThick);
            }
          } else {
            if (kCfgOptBoxFill)
              drawList->AddRectFilled({boxLeft, boxTop}, {boxRight, boxBtm},
                                      curBoxFill);
            if (kCfgOptBoxOutline)
              drawList->AddRect({boxLeft - 1, boxTop - 1},
                                {boxRight + 1, boxBtm + 1}, kBoxOutl, 0, 0,
                                kBThick + 1.6f);
            drawList->AddRect({boxLeft, boxTop}, {boxRight, boxBtm},
                              curBoxColor, 0, 0, kBThick);
          }
        }

        // 2. Health bar
        if (drawHealth) {
          float bL = (kCfgHpPos == 1)
                         ? (boxRight + 4.0f)
                         : (boxLeft - kHpBW - 4.0f);
          float pct = std::clamp(ent.health * 0.01f, 0.0f, 1.0f);
          float hH = boxH * pct;
          ImU32 hpCol =
              kCfgHpGrad
                  ? ((pct > 0.5f) ? IM_COL32(0, 255, 120, 255)
                                  : (pct > 0.25f ? IM_COL32(255, 180, 0, 255)
                                                 : IM_COL32(255, 40, 40, 255)))
                  : kCfgHpSolidCol;
          if (kCfgHealthType == 0 ||
              kCfgHealthType == 1) {
            drawList->AddRectFilled({bL - 1, boxTop - 1},
                                    {bL + kHpBW + 1, boxBtm + 1}, kBarOutl);
            drawList->AddRectFilled({bL, boxBtm - hH}, {bL + kHpBW, boxBtm},
                                    hpCol);
          }
          if (kCfgHealthType == 0 ||
              kCfgHealthType == 2) {
            char hpTxt[8];
            snprintf(hpTxt, sizeof(hpTxt), "%d", ent.health);
            float tx = (kCfgHealthType == 2) ? (boxLeft - 24.0f)
                                             : (bL - 2.0f);
            float ty = (kCfgHealthType == 2) ? (boxTop + 2.0f)
                                             : (boxBtm + 2.0f);
            if (ty + 14.f < (float)screenHeight)
              drawList->AddText({tx, ty}, IM_COL32(220, 220, 220, 240), hpTxt);
          }
        }

        // 3. Armor bar
        if (drawArmor && ent.armor > 0) {
          float bW = 3.5f, bR = boxRight + 4.0f;
          float pct = std::clamp(ent.armor * 0.01f, 0.0f, 1.0f);
          float aH = boxH * pct;
          drawList->AddRectFilled({bR - 1, boxTop - 1},
                                  {bR + bW + 1, boxBtm + 1}, kBarOutl);
          drawList->AddRectFilled({bR, boxBtm - aH}, {bR + bW, boxBtm},
                                  IM_COL32(80, 180, 255, 230));
        }
        // 3b. Ammo bar
        if (drawAmmoBar && ent.ammo >= 0 && ent.ammo <= 150) {
          float bH = 3.0f, bY = boxBtm + 4.0f;
          float pct = std::clamp(ent.ammo * 0.0333f, 0.0f, 1.0f);
          float aW = boxW * pct;
          drawList->AddRectFilled({boxLeft - 1, bY - 1},
                                  {boxRight + 1, bY + bH + 1}, kBarOutl);
          drawList->AddRectFilled({boxLeft, bY}, {boxLeft + aW, bY + bH},
                                  IM_COL32(175, 95, 255, 230));
        }

        // 4. Head pos
        if (drawHeadDot) {
          float r = std::max(2.5f, boxH * 0.045f);
          drawList->AddCircleFilled({headScr.x, headScr.y}, r + 1.2f,
                                    IM_COL32(0, 0, 0, 220));
          drawList->AddCircleFilled({headScr.x, headScr.y}, r, kCfgHeadCol);
        }

        // 5. Skeleton ESP
        if (drawSkeletons && ent.hasBones) {
          static constexpr struct {
            uint8_t a, b;
          } kLinksOpt[] = {{7, 6}, {6, 1}, {6, 11}, {6, 15}, {1, 19}, {1, 22}};
          static constexpr uint8_t kUniqueOpt[] = {1, 6, 7, 11, 15, 19, 22};
          static constexpr struct {
            uint8_t a, b;
          } kLinksFull[] = {
              {7, 6},   // Head -> Neck
              {6, 4},   // Neck -> Spine_2 (Chest)
              {4, 3},   // Spine_2 -> Spine_1
              {3, 1},   // Spine_1 -> Pelvis
              // Left Arm
              {6, 9},   // Neck -> Left Shoulder / Bicep
              {9, 10},  // Left Bicep -> Left Forearm
              {10, 11}, // Left Forearm -> Left Hand
              // Right Arm
              {6, 13},  // Neck -> Right Shoulder / Bicep
              {13, 14}, // Right Bicep -> Right Forearm
              {14, 15}, // Right Forearm -> Right Hand
              // Left Leg
              {1, 17},  // Pelvis -> Left Thigh
              {17, 18}, // Left Thigh -> Left Knee / Shin
              {18, 19}, // Left Shin -> Left Ankle
              // Right Leg
              {1, 20},  // Pelvis -> Right Thigh
              {20, 21}, // Right Thigh -> Right Knee / Shin
              {21, 22}  // Right Shin -> Right Ankle
          };
          static constexpr uint8_t kUniqueFull[] = {
              1, 3, 4, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18, 19, 20, 21, 22};
          const uint8_t *uniqueBones =
              (skeletonStyle == 0) ? kUniqueOpt : kUniqueFull;
          size_t uniqueCount =
              (skeletonStyle == 0) ? sizeof(kUniqueOpt) : sizeof(kUniqueFull);

          Vec2 scr[32];
          bool ok[32] = {false};
          if (hasVel) {
            Vec3 extBones[32]{};
            for (int bi = 0; bi < 32; bi++) {
              const Vec3 &bw = ent.bones[bi];
              extBones[bi] =
                  (bw.x || bw.y || bw.z)
                      ? (bw + (ent.velocity * dtSnap))
                      : bw;
            }
            ProjectBonesChunked(extBones, uniqueBones, g_w2sCache, scr, ok,
                                uniqueCount);
          } else {
            ProjectBonesChunked(ent.bones, uniqueBones, g_w2sCache, scr, ok, uniqueCount);
          }

          uint8_t skelA =
              (uint8_t)std::clamp(220.0f - distU * 0.04f, 100.0f, 220.0f);
          bool drawShadow = (distU < 1200.0f); // OPT: gate shadow by distance
          ImU32 shadowCol = IM_COL32(0, 0, 0, (uint8_t)(skelA * 0.7f));
          ImU32 skelCol;
          if (kVisCheck && !ent.spotted && !ent.spottedByLocal)
            skelCol = IM_COL32(kCfgSkelOccl0, kCfgSkelOccl1, kCfgSkelOccl2,
                               (uint8_t)(kCfgSkelOccl3 * skelA / 255));
          else
            skelCol = IM_COL32(kCfgSkelCol0, kCfgSkelCol1, kCfgSkelCol2,
                               (uint8_t)(kCfgSkelCol3 * skelA / 255));

          auto renderLinks = [&](const auto *links, size_t count) {
            if (drawShadow) {
              for (size_t i = 0; i < count; i++) {
                const auto &lnk = links[i];
                if (!ok[lnk.a] || !ok[lnk.b])
                  continue;
                drawList->AddLine({scr[lnk.a].x + 1.f, scr[lnk.a].y + 1.f},
                                  {scr[lnk.b].x + 1.f, scr[lnk.b].y + 1.f},
                                  shadowCol, kSkThick + 1.0f);
              }
            }
            for (size_t i = 0; i < count; i++) {
              const auto &lnk = links[i];
              if (!ok[lnk.a] || !ok[lnk.b])
                continue;
              drawList->AddLine({scr[lnk.a].x, scr[lnk.a].y},
                                {scr[lnk.b].x, scr[lnk.b].y}, skelCol,
                                kSkThick);
            }
          };
          if (skeletonStyle == 0)
            renderLinks(kLinksOpt, sizeof(kLinksOpt) / sizeof(kLinksOpt[0]));
          else
            renderLinks(kLinksFull, sizeof(kLinksFull) / sizeof(kLinksFull[0]));

          if (g_Config.skeleton.joints) {
            for (size_t j = 0; j < uniqueCount; j++) {
              uint8_t bi = uniqueBones[j];
              if (ok[bi])
                drawList->AddCircleFilled(
                    {scr[bi].x, scr[bi].y}, 2.2f,
                    IM_COL32(255, 255, 255, (uint8_t)(skelA * 0.9f)));
            }
          }
          if (drawHeadDot && g_Config.skeleton.head_circle && ok[7]) {
            float headR = std::max(4.0f, boxH * 0.055f);
            if (drawShadow)
              drawList->AddCircle({scr[7].x, scr[7].y}, headR, shadowCol, 16,
                                  kSkThick + 1.2f);
            drawList->AddCircle({scr[7].x, scr[7].y}, headR, skelCol, 16,
                                kSkThick);
          }
        }

        // 5b. Chams
        if (chamsEnabled) {
          bool isVis = ent.spotted || ent.spottedByLocal;
          float *colArr = isVis ? g_Config.visuals.chams_visible_color
                                : g_Config.visuals.chams_occluded_color;
          float alpha =
              std::clamp(g_Config.visuals.chams_opacity, 0.10f, 1.00f);
          ImU32 cCol = IM_COL32(
              (uint8_t)(colArr[0] * 255.f), (uint8_t)(colArr[1] * 255.f),
              (uint8_t)(colArr[2] * 255.f), (uint8_t)(alpha * 255.f));
          ImU32 cBg = IM_COL32(
              (uint8_t)(colArr[0] * 255.f), (uint8_t)(colArr[1] * 255.f),
              (uint8_t)(colArr[2] * 255.f), (uint8_t)(alpha * 90.f));
          float hR = std::max(5.0f, boxH * 0.06f);
          if (chamsType == 0) {
            drawList->AddCircleFilled({headScr.x, headScr.y}, hR, cCol);
            drawList->AddRectFilled({boxLeft + boxW * 0.10f, headScr.y + hR},
                                    {boxRight - boxW * 0.10f, boxBtm}, cBg,
                                    6.0f);
            drawList->AddRect({boxLeft + boxW * 0.10f, headScr.y + hR},
                              {boxRight - boxW * 0.10f, boxBtm}, cCol, 6.0f, 0,
                              1.5f);
          } else if (chamsType == 1) {
            drawList->AddCircle({headScr.x, headScr.y}, hR, cCol, 16, 1.8f);
            drawList->AddLine({boxLeft, boxTop}, {boxRight, boxBtm}, cCol,
                              1.2f);
            drawList->AddLine({boxRight, boxTop}, {boxLeft, boxBtm}, cCol,
                              1.2f);
            drawList->AddRect({boxLeft, boxTop}, {boxRight, boxBtm}, cCol, 0, 0,
                              1.5f);
          } else if (chamsType == 2) {
            drawList->AddCircleFilled({headScr.x, headScr.y}, hR, cBg);
            drawList->AddCircle({headScr.x, headScr.y}, hR, cCol, 16, 2.0f);
            drawList->AddRectFilled({boxLeft, boxTop}, {boxRight, boxBtm}, cBg,
                                    4.0f);
            drawList->AddRect({boxLeft, boxTop}, {boxRight, boxBtm}, cCol, 4.0f,
                              0, 2.0f);
          } else if (chamsType == 3) {
            static float pulseMs = 0.0f;
            pulseMs += 0.03f;
            float gR = hR + 2.5f * sinf(pulseMs);
            drawList->AddCircle({headScr.x, headScr.y}, gR, cCol, 20, 2.5f);
            drawList->AddRect({boxLeft - 2.0f, boxTop - 2.0f},
                              {boxRight + 2.0f, boxBtm + 2.0f}, cCol, 6.0f, 0,
                              2.5f);
          }
        }

        // 6. Name + distance
        if (drawNames || drawDistance) {
          char lblBuf[128] = {};
          if (drawNames && drawDistance)
            snprintf(lblBuf, sizeof(lblBuf), "%s  %.0fm", ent.name, distM);
          else if (drawNames)
            snprintf(lblBuf, sizeof(lblBuf), "%s", ent.name);
          else
            snprintf(lblBuf, sizeof(lblBuf), "%.0fm", distM);
          ImVec2 tsz =
              Inter_S_3 ? Inter_S_3->CalcTextSizeA(15.f, FLT_MAX, 0.0f, lblBuf)
                        : ImGui::CalcTextSize(lblBuf);
          float tx = originScr.x - tsz.x * 0.5f;
          float ty = boxTop - tsz.y - 3.0f;
          if (Inter_S_3) {
            if (!farLOD)
              drawList->AddText(Inter_S_3, 15.f, {tx + 1.f, ty + 1.f},
                                IM_COL32(0, 0, 0, 220), lblBuf);
            drawList->AddText(Inter_S_3, 15.f, {tx, ty}, kCfgNameCol, lblBuf);
          } else {
            if (!farLOD)
              drawList->AddText({tx + 1.f, ty + 1.f}, IM_COL32(0, 0, 0, 220),
                                lblBuf);
            drawList->AddText({tx, ty}, kCfgNameCol, lblBuf);
          }
        }

        // 7. Bomb Carrier
        if (g_Config.visuals.bomb_carrier && ent.hasBomb) {
          const char *c4Tag = "[BOMB CARRIER]";
          float c4Tx = originScr.x - s_c4TagSize.x * 0.5f;
          float c4Ty = boxTop - s_c4TagSize.y - 6.0f;
          if (drawNames)
            c4Ty -= 18.0f;
          drawList->AddRectFilled(
              {c4Tx - 6.0f, c4Ty - 2.0f},
              {c4Tx + s_c4TagSize.x + 6.0f, c4Ty + s_c4TagSize.y + 2.0f},
              IM_COL32(28, 12, 12, 230), 4.0f);
          drawList->AddRect(
              {c4Tx - 6.0f, c4Ty - 2.0f},
              {c4Tx + s_c4TagSize.x + 6.0f, c4Ty + s_c4TagSize.y + 2.0f},
              kCfgBombBorder, 4.0f, 0, 1.2f);
          if (Inter_S_3) {
            if (!farLOD)
              drawList->AddText(Inter_S_3, 15.f, {c4Tx + 1.f, c4Ty + 1.f},
                                IM_COL32(0, 0, 0, 220), c4Tag);
            drawList->AddText(Inter_S_3, 15.f, {c4Tx, c4Ty},
                              IM_COL32(255, 215, 0, 255), c4Tag);
          } else {
            if (!farLOD)
              drawList->AddText({c4Tx + 1.f, c4Ty + 1.f},
                                IM_COL32(0, 0, 0, 220), c4Tag);
            drawList->AddText({c4Tx, c4Ty}, IM_COL32(255, 215, 0, 255), c4Tag);
          }
        }

        // 8. Weapon
        if (drawWeapon && ent.weaponName[0]) {
          ImVec2 wpnSz = Inter_S_3 ? Inter_S_3->CalcTextSizeA(
                                         14.f, FLT_MAX, 0.0f, ent.weaponName)
                                   : ImGui::CalcTextSize(ent.weaponName);
          float wpnTx = originScr.x - wpnSz.x * 0.5f;
          float wpnTy = boxBtm + 4.0f;
          if (drawAmmoBar)
            wpnTy += 6.0f;
          if (Inter_S_3) {
            if (!farLOD)
              drawList->AddText(Inter_S_3, 14.f, {wpnTx + 1.f, wpnTy + 1.f},
                                IM_COL32(0, 0, 0, 220), ent.weaponName);
            drawList->AddText(Inter_S_3, 14.f, {wpnTx, wpnTy}, kCfgWpnCol,
                              ent.weaponName);
          } else {
            if (!farLOD)
              drawList->AddText({wpnTx + 1.f, wpnTy + 1.f},
                                IM_COL32(0, 0, 0, 220), ent.weaponName);
            drawList->AddText({wpnTx, wpnTy}, kCfgWpnCol, ent.weaponName);
          }
        }

        // 8b. Snaplines
        if (drawSnaplines)
          drawList->AddLine({screenWidth / 2.0f, (float)screenHeight},
                            {originScr.x, originScr.y}, kCfgSnapCol, 1.2f);

        // 9. Eye ray (OPT: kRayLen 500u instead of 120u)
        if (drawEyeRay) {
          float pitch = ent.eyeAngles.x * (3.14159f / 180.0f);
          float yaw = ent.eyeAngles.y * (3.14159f / 180.0f);
          float cp = std::cos(pitch), sp = std::sin(pitch);
          float cy = std::cos(yaw), sy = std::sin(yaw);
          constexpr float kRayLen = 500.0f;
          Vec3 endW = {ent.headPos.x + cp * cy * kRayLen,
                       ent.headPos.y + cp * sy * kRayLen,
                       ent.headPos.z - sp * kRayLen};
          Vec2 endScr;
          if (WorldToScreen(endW, liveVM, endScr))
            drawList->AddLine({headScr.x, headScr.y}, {endScr.x, endScr.y},
                              kCfgEyeCol, 1.3f);
        }

        // 9b. Flags
        if (drawFlags) {
          float flagX = boxRight + (drawArmor && ent.armor > 0 ? 9.0f : 5.0f);
          float flagY = boxTop + 2.0f;
          auto renderFlag = [&](const char *txt, ImU32 col) {
            if (flagY + 11.0f > boxBtm + 24.0f)
              return;
            if (Inter_S_3) {
              if (!farLOD)
                drawList->AddText(Inter_S_3, 11.0f,
                                  ImVec2(flagX + 1.0f, flagY + 1.0f),
                                  IM_COL32(0, 0, 0, 220), txt);
              drawList->AddText(Inter_S_3, 11.0f, ImVec2(flagX, flagY), col,
                                txt);
            } else {
              if (!farLOD)
                drawList->AddText(ImVec2(flagX + 1.0f, flagY + 1.0f),
                                  IM_COL32(0, 0, 0, 220), txt);
              drawList->AddText(ImVec2(flagX, flagY), col, txt);
            }
            flagY += 10.0f;
          };
          if (ent.hasBomb)
            renderFlag("C4", IM_COL32(255, 65, 65, 255));
          if (ent.armor > 0)
            renderFlag("HK", IM_COL32(80, 195, 255, 240));
          if (ent.shotsFired > 0)
            renderFlag("FIRE", IM_COL32(255, 140, 30, 255));
          if (ent.spotted)
            renderFlag("VIS", IM_COL32(40, 255, 120, 240));
          if (hasVel) {
            float spd = sqrtf(ent.velocity.x * ent.velocity.x +
                              ent.velocity.y * ent.velocity.y);
            if (spd > 215.0f)
              renderFlag("RUN", IM_COL32(255, 220, 80, 230));
            else if (spd < 15.0f)
              renderFlag("STILL", IM_COL32(180, 180, 190, 200));
          }
        }
      }

      // 10. Off-screen Radar Arrows
      if (offscreenEsp) {
        const float cx = screenWidth * 0.5f, cy = screenHeight * 0.5f;
        const float arrowRadius = std::min(cx, cy) * 0.72f;
        for (int i = 0; i < snap.count; i++) {
          const auto &ent = snap.entities[i];
          if (!ent.valid)
            continue;
          if (teamCheck && localTeam && ent.team == localTeam)
            continue;
          Vec2 scr;
          if (WorldToScreen(ent.origin, liveVM, scr)) {
            if (scr.x >= 20.0f && scr.x <= screenWidth - 20.0f &&
                scr.y >= 20.0f && scr.y <= screenHeight - 20.0f)
              continue;
          }
          float dx = ent.origin.x - localOrigin.x;
          float dy = ent.origin.y - localOrigin.y;
          float dz = ent.origin.z - localOrigin.z;
          float camRight =
              dx * liveVM.m[0][0] + dy * liveVM.m[0][1] + dz * liveVM.m[0][2];
          float camForward =
              dx * liveVM.m[2][0] + dy * liveVM.m[2][1] + dz * liveVM.m[2][2];
          float relAngle = std::atan2(-camForward, camRight);
          float cosA = std::cos(relAngle), sinA = std::sin(relAngle);
          float tipX = cx + cosA * arrowRadius;
          float tipY = cy + sinA * arrowRadius;
          constexpr float arrowLen = 15.0f, arrowBase = 8.0f;
          float backX = tipX - cosA * arrowLen;
          float backY = tipY - sinA * arrowLen;
          float perpX = -sinA * arrowBase;
          float perpY = cosA * arrowBase;
          ImVec2 pTip = {tipX, tipY};
          ImVec2 pLeft = {backX + perpX, backY + perpY};
          ImVec2 pRight = {backX - perpX, backY - perpY};
          float distM = localOrigin.DistTo(ent.origin) * 0.0254f;
          uint8_t alpha =
              (uint8_t)std::clamp(255.0f - distM * 2.2f, 90.0f, 240.0f);
          ImU32 arrCol = IM_COL32(255, 60, 80, alpha);
          ImU32 arrOutline = IM_COL32(0, 0, 0, (uint8_t)(alpha * 0.8f));
          drawList->AddTriangleFilled(pTip, pLeft, pRight, arrCol);
          drawList->AddTriangle(pTip, pLeft, pRight, arrOutline, 1.5f);
        }
      }
    }

    // ==== SOUND ESP ====
    if (soundEsp && g_masterEnabled) {
      static float kRingCos[24], kRingSin[24];
      static bool kRingInit = false;
      if (!kRingInit) {
        constexpr float kStep = 2.0f * 3.14159265f / 24.0f;
        for (int i = 0; i < 24; i++) {
          kRingCos[i] = std::cos(i * kStep);
          kRingSin[i] = std::sin(i * kStep);
        }
        kRingInit = true;
      }
      uint64_t nowMs = GetTickCount64();
      const float cx = screenWidth * 0.5f, cy = screenHeight * 0.5f;
      const float soundArrowRadius = std::min(cx, cy) * 0.68f;
      for (int slot = 1; slot <= 64; slot++) {
        uint64_t lastMs = g_soundWallMs[slot].load(std::memory_order_acquire);
        if (!lastMs)
          continue;
        int64_t elapsed = (int64_t)(nowMs - lastMs);
        if (elapsed < 0 || elapsed > 2400)
          continue;
        const auto &snd = g_soundEvents[slot];
        if (teamCheck && localTeam && snd.team == localTeam)
          continue;
        if (!snd.origin.x && !snd.origin.y && !snd.origin.z)
          continue;
        float life = 1.0f - (float)elapsed / 2400.0f;
        float distM = localOrigin.DistTo(snd.origin) * 0.0254f;
        Vec2 centerScr{};
        bool centerOnScreen = WorldToScreen(snd.origin, liveVM, centerScr);
        if (centerOnScreen && centerScr.x >= -80.0f &&
            centerScr.x <= screenWidth + 80.0f && centerScr.y >= -80.0f &&
            centerScr.y <= screenHeight + 80.0f) {
          float r1 = 30.0f + (float)elapsed * 0.045f;
          float r2 = r1 - 26.0f;
          ImU32 col1 = IM_COL32(255, 145, 30, (int)(life * 210.0f));
          Vec2 prev1;
          bool prevOk1 = false;
          for (int s = 0; s <= 24; s++) {
            int idx = s % 24;
            Vec3 wp = {snd.origin.x + kRingCos[idx] * r1,
                       snd.origin.y + kRingSin[idx] * r1, snd.origin.z};
            Vec2 sp;
            bool valid = WorldToScreen(wp, liveVM, sp);
            if (valid && prevOk1)
              drawList->AddLine({prev1.x, prev1.y}, {sp.x, sp.y}, col1, 1.8f);
            prev1 = sp;
            prevOk1 = valid;
          }
          if (r2 > 5.0f) {
            float echoLife = life * 0.60f;
            ImU32 col2 = IM_COL32(255, 200, 80, (int)(echoLife * 160.0f));
            Vec2 prev2;
            bool prevOk2 = false;
            for (int s = 0; s <= 24; s++) {
              int idx = s % 24;
              Vec3 wp = {snd.origin.x + kRingCos[idx] * r2,
                         snd.origin.y + kRingSin[idx] * r2, snd.origin.z};
              Vec2 sp;
              bool valid = WorldToScreen(wp, liveVM, sp);
              if (valid && prevOk2)
                drawList->AddLine({prev2.x, prev2.y}, {sp.x, sp.y}, col2, 1.2f);
              prev2 = sp;
              prevOk2 = valid;
            }
          }
          drawList->AddCircleFilled(
              {centerScr.x, centerScr.y}, 3.5f,
              IM_COL32(255, 230, 100, (int)(life * 240.0f)));
          drawList->AddCircle({centerScr.x, centerScr.y}, 6.5f,
                              IM_COL32(255, 160, 40, (int)(life * 180.0f)), 12,
                              1.0f);
          char badgeText[32];
          snprintf(badgeText, sizeof(badgeText), "SOUND %.0fm", distM);
          ImVec2 tsz = ImGui::CalcTextSize(badgeText);
          float bx = centerScr.x - tsz.x * 0.5f;
          float by = centerScr.y - 18.0f;
          drawList->AddRectFilled(
              {bx - 4, by - 2}, {bx + tsz.x + 4, by + tsz.y + 2},
              IM_COL32(10, 10, 15, (int)(life * 180.0f)), 3.0f);
          drawList->AddRect({bx - 4, by - 2}, {bx + tsz.x + 4, by + tsz.y + 2},
                            IM_COL32(255, 160, 40, (int)(life * 150.0f)), 3.0f,
                            0, 1.0f);
          drawList->AddText({bx, by},
                            IM_COL32(255, 220, 150, (int)(life * 255.0f)),
                            badgeText);
        } else {
          float dx = snd.origin.x - localOrigin.x;
          float dy = snd.origin.y - localOrigin.y;
          float dz = snd.origin.z - localOrigin.z;
          float camRight =
              dx * liveVM.m[0][0] + dy * liveVM.m[0][1] + dz * liveVM.m[0][2];
          float camForward =
              dx * liveVM.m[2][0] + dy * liveVM.m[2][1] + dz * liveVM.m[2][2];
          float relAngle = std::atan2(-camForward, camRight);
          float cosA = std::cos(relAngle), sinA = std::sin(relAngle);
          float tipX = cx + cosA * soundArrowRadius;
          float tipY = cy + sinA * soundArrowRadius;
          constexpr float arrowLen = 14.0f, arrowBase = 7.0f;
          float backX = tipX - cosA * arrowLen;
          float backY = tipY - sinA * arrowLen;
          float perpX = -sinA * arrowBase;
          float perpY = cosA * arrowBase;
          ImVec2 pTip = {tipX, tipY};
          ImVec2 pLeft = {backX + perpX, backY + perpY};
          ImVec2 pRight = {backX - perpX, backY - perpY};
          uint8_t aByte = (uint8_t)(life * 220.0f);
          ImU32 arrCol = IM_COL32(255, 150, 20, aByte);
          ImU32 arrOutline = IM_COL32(0, 0, 0, (uint8_t)(aByte * 0.8f));
          drawList->AddTriangleFilled(pTip, pLeft, pRight, arrCol);
          drawList->AddTriangle(pTip, pLeft, pRight, arrOutline, 1.5f);
          char offText[16];
          snprintf(offText, sizeof(offText), "%.0fm", distM);
          drawList->AddText({backX + cosA * 4.0f, backY + sinA * 4.0f},
                            IM_COL32(255, 230, 160, aByte), offText);
        }
      }
    }

    // ==== FOV CIRCLE ====
    if (aimbotFovCircle && g_masterEnabled && activeWcfg.enabled) {
      float fovDeg = (activeWcfg.fov > 0.1f) ? activeWcfg.fov : 5.0f;
      float fovPx = fovDeg * ((float)screenHeight / 90.0f);
      drawList->AddCircle({screenWidth / 2.0f, screenHeight / 2.0f}, fovPx,
                          IM_COL32(255, 255, 255, 90), 40, 1.2f);
    }

    // ==== HITMARKER ====
    if (g_Config.visuals.hitmarker) {
      uint64_t hmTime = g_hitmarkerTime.load(std::memory_order_acquire);
      if (hmTime) {
        uint64_t nowMs = GetTickCount64();
        int64_t elapsed = (int64_t)(nowMs - hmTime);
        if (elapsed >= 0 && elapsed <= 550) {
          float alpha = 1.0f - (float)elapsed / 550.0f;
          float cx = screenWidth * 0.5f, cy = screenHeight * 0.5f;
          int dmg = g_hitmarkerDamage.load(std::memory_order_relaxed);
          constexpr float hmGap = 4.0f, hmLim = 10.0f;
          ImU32 hmCol = IM_COL32(255, 255, 255, (int)(alpha * 240.0f));
          ImU32 hmShadow = IM_COL32(0, 0, 0, (int)(alpha * 200.0f));
          auto drawHmLine = [&](float x1, float y1, float x2, float y2) {
            drawList->AddLine({x1 + 1.0f, y1 + 1.0f}, {x2 + 1.0f, y2 + 1.0f},
                              hmShadow, 2.5f);
            drawList->AddLine({x1, y1}, {x2, y2}, hmCol, 1.8f);
          };
          drawHmLine(cx - hmLim, cy - hmLim, cx - hmGap, cy - hmGap);
          drawHmLine(cx + hmGap, cy - hmGap, cx + hmLim, cy - hmLim);
          drawHmLine(cx - hmLim, cy + hmLim, cx - hmGap, cy + hmGap);
          drawHmLine(cx + hmGap, cy + hmLim, cx + hmLim, cy + hmLim);
          if (dmg > 0) {
            char dmgBuf[16];
            snprintf(dmgBuf, sizeof(dmgBuf), "-%d", dmg);
            float dmgY = cy - 22.0f - (float)elapsed * 0.05f;
            ImVec2 tsz = ImGui::CalcTextSize(dmgBuf);
            float dmgX = cx - tsz.x * 0.5f;
            ImU32 dmgCol =
                (dmg >= 100)  ? IM_COL32(255, 45, 45, (int)(alpha * 255.0f))
                : (dmg >= 50) ? IM_COL32(255, 185, 25, (int)(alpha * 255.0f))
                              : IM_COL32(255, 230, 100, (int)(alpha * 255.0f));
            drawList->AddText({dmgX + 1.0f, dmgY + 1.0f},
                              IM_COL32(0, 0, 0, (int)(alpha * 220.0f)), dmgBuf);
            drawList->AddText({dmgX, dmgY}, dmgCol, dmgBuf);
          }
        }
      }
    }

    // ==== SNIPER NO-SCOPE CROSSHAIR ====
    if (g_masterEnabled && g_Config.visuals.sniper_crosshair && snap.localWeaponDefIdx > 0) {
      uint16_t wId = snap.localWeaponDefIdx;
      bool isSniper = (wId == 9 || wId == 11 || wId == 38 || wId == 40);
      if (isSniper) {
        float cx = screenWidth * 0.5f, cy = screenHeight * 0.5f;
        drawList->AddLine({cx - 7.0f, cy}, {cx + 8.0f, cy}, IM_COL32(0, 0, 0, 220), 2.5f);
        drawList->AddLine({cx, cy - 7.0f}, {cx, cy + 8.0f}, IM_COL32(0, 0, 0, 220), 2.5f);
        drawList->AddLine({cx - 6.0f, cy}, {cx + 7.0f, cy}, IM_COL32(255, 65, 65, 240), 1.5f);
        drawList->AddLine({cx, cy - 6.0f}, {cx, cy + 7.0f}, IM_COL32(255, 65, 65, 240), 1.5f);
        drawList->AddCircleFilled({cx, cy}, 1.5f, IM_COL32(255, 255, 255, 240));
      }
    }

    // ==== RECOIL CROSSHAIR ====
    if (g_masterEnabled && g_Config.visuals.recoil_crosshair) {
      float fovScale = (float)screenHeight / 90.0f;
      float rcsX =
          (screenWidth * 0.5f) - (snap.localPunchAngle.y * kRcsPunchScale) * fovScale;
      float rcsY =
          (screenHeight * 0.5f) + (snap.localPunchAngle.x * kRcsPunchScale) * fovScale;
      drawList->AddLine({rcsX - 6.0f, rcsY}, {rcsX + 7.0f, rcsY},
                        IM_COL32(0, 0, 0, 220), 2.5f);
      drawList->AddLine({rcsX, rcsY - 6.0f}, {rcsX, rcsY + 7.0f},
                        IM_COL32(0, 0, 0, 220), 2.5f);
      drawList->AddLine({rcsX - 5.0f, rcsY}, {rcsX + 6.0f, rcsY},
                        IM_COL32(50, 230, 120, 240), 1.4f);
      drawList->AddLine({rcsX, rcsY - 5.0f}, {rcsX, rcsY + 6.0f},
                        IM_COL32(50, 230, 120, 240), 1.4f);
      drawList->AddCircleFilled({rcsX, rcsY}, 1.5f,
                                IM_COL32(255, 255, 255, 240));
    }

    // ==== SPECTATOR ALERT ====
    if (showSpectators && slowLocal.spectatorCount > 0 && !menuOpen) {
      char specBuf[64];
      snprintf(specBuf, sizeof(specBuf), "SPECTATORS (%d)",
               slowLocal.spectatorCount);
      float pillW = 165.0f;
      float pillX = (float)screenWidth - pillW - 20.0f;
      float pillY = 55.0f;
      float totalH = 26.0f + slowLocal.spectatorCount * 18.0f;
      drawList->AddRectFilled({pillX, pillY}, {pillX + pillW, pillY + totalH},
                              IM_COL32(18, 20, 26, 220), 8.0f);
      drawList->AddRect({pillX, pillY}, {pillX + pillW, pillY + totalH},
                        IM_COL32(239, 68, 68, 180), 8.0f, 0, 1.2f);
      drawList->AddCircleFilled({pillX + 14.0f, pillY + 13.0f}, 4.0f,
                                IM_COL32(239, 68, 68, 255));
      drawList->AddText({pillX + 24.0f, pillY + 6.0f},
                        IM_COL32(255, 255, 255, 240), specBuf);
      for (int s = 0; s < slowLocal.spectatorCount; s++) {
        float sY = pillY + 24.0f + s * 18.0f;
        drawList->AddText({pillX + 14.0f, sY}, IM_COL32(200, 200, 210, 220),
                          slowLocal.spectators[s].name);
      }
    }

    // ==== BOMB ESP ====
    if (bombEsp && g_masterEnabled && slowLocal.bomb.valid &&
        slowLocal.bomb.ticking) {
      auto now = std::chrono::steady_clock::now();
      float dt2 = std::chrono::duration<float>(now - slowLocal.bomb.captureTime)
                      .count();
      if (dt2 < 0.0f)
        dt2 = 0.0f;
      float remaining = slowLocal.bomb.remainingBlow - dt2;
      if (remaining <= 0.0f || slowLocal.bomb.remainingBlow <= 0.0f) {
        float elapsed2 = slowLocal.bomb.wallTimeCaptured
                             ? std::chrono::duration<float>(
                                   now - slowLocal.bomb.plantWallTime)
                                   .count()
                             : 0.f;
        remaining = std::max(0.0f, slowLocal.bomb.timerLength - elapsed2);
      }
      if (remaining < 0.0f)
        remaining = 0.0f;
      float totalTimer = (slowLocal.bomb.timerLength > 0.0f)
                             ? slowLocal.bomb.timerLength
                             : 40.0f;
      float pct = std::clamp(remaining / totalTimer, 0.0f, 1.0f);
      const char *siteStr = (slowLocal.bomb.site == 0)   ? "A"
                            : (slowLocal.bomb.site == 1) ? "B"
                                                         : "?";
      char bombBuf[96];
      ImU32 bombCol = (pct > 0.5f)    ? IM_COL32(16, 185, 129, 255)
                      : (pct > 0.25f) ? IM_COL32(245, 158, 11, 255)
                                      : IM_COL32(239, 68, 68, 255);
      float remainingDefuse =
          std::max(0.0f, slowLocal.bomb.remainingDefuse - dt2);
      bool isDefusing = slowLocal.bomb.defusing;
      bool canDefuse =
          slowLocal.bomb.canDefuseInTime && (remainingDefuse <= remaining);
      if (slowLocal.bomb.defused) {
        snprintf(bombBuf, sizeof(bombBuf), "[C4 DEFUSED]");
        bombCol = IM_COL32(14, 165, 233, 255);
      } else if (isDefusing) {
        if (canDefuse) {
          snprintf(bombBuf, sizeof(bombBuf),
                   "[SITE %s] DEFUSING: %.1fs (Bomb: %.1fs)", siteStr,
                   remainingDefuse, remaining);
          bombCol = IM_COL32(14, 165, 233, 255);
        } else {
          snprintf(bombBuf, sizeof(bombBuf),
                   "[SITE %s] CANNOT DEFUSE! (%.1fs < %.1fs)", siteStr,
                   remaining, remainingDefuse);
          bombCol = IM_COL32(255, 30, 60, 255);
        }
      } else
        snprintf(bombBuf, sizeof(bombBuf), "[BOMB %s] %.1fs DETONATION",
                 siteStr, remaining);
      ImVec2 tsz = ImGui::CalcTextSize(bombBuf);
      float bx = screenWidth / 2.0f - tsz.x / 2.0f;
      float by = (float)screenHeight - 80.0f;
      drawList->AddRectFilled({bx - 12.0f, by - 6.0f},
                              {bx + tsz.x + 12.0f, by + tsz.y + 24.0f},
                              IM_COL32(10, 12, 16, 220), 6.0f);
      drawList->AddRect({bx - 12.0f, by - 6.0f},
                        {bx + tsz.x + 12.0f, by + tsz.y + 24.0f}, bombCol, 6.0f,
                        0, 1.2f);
      drawList->AddText({bx, by}, bombCol, bombBuf);
      float barW = 260.0f, barH = 6.0f;
      float barX = screenWidth / 2.0f - barW / 2.0f, barY = by + tsz.y + 8.0f;
      drawList->AddRectFilled({barX, barY}, {barX + barW, barY + barH},
                              IM_COL32(24, 28, 36, 220), 3.0f);
      drawList->AddRectFilled({barX, barY}, {barX + barW * pct, barY + barH},
                              bombCol, 3.0f);
      if (isDefusing && slowLocal.bomb.defuseLength > 0.0f) {
        float defusePct = std::clamp(
            1.0f - (remainingDefuse / slowLocal.bomb.defuseLength), 0.0f, 1.0f);
        float defBarY = barY + barH + 4.0f;
        ImU32 defCol = canDefuse ? IM_COL32(14, 165, 233, 255)
                                 : IM_COL32(255, 30, 30, 255);
        drawList->AddRectFilled({barX, defBarY}, {barX + barW, defBarY + 4.0f},
                                IM_COL32(15, 25, 35, 220), 2.0f);
        drawList->AddRectFilled({barX, defBarY},
                                {barX + barW * defusePct, defBarY + 4.0f},
                                defCol, 2.0f);
      }
      Vec2 c4Scr;
      if (WorldToScreen(slowLocal.bomb.worldPos, liveVM, c4Scr)) {
        drawList->AddCircleFilled({c4Scr.x, c4Scr.y}, 8.0f, bombCol);
        drawList->AddCircle({c4Scr.x, c4Scr.y}, 14.0f, bombCol, 16, 1.5f);
        drawList->AddText({c4Scr.x + 16.0f, c4Scr.y - 8.0f}, bombCol, bombBuf);
      }
      g_Config.island.bomb_active = true;
      g_Config.island.bomb_timer = remaining;
      g_Config.island.bomb_site = slowLocal.bomb.site;
      g_Config.island.defusing = isDefusing;
      g_Config.island.defuse_timer = remainingDefuse;
    } else {
      g_Config.island.bomb_active = false;
      g_Config.island.defusing = false;
    }

    // ==== WORLD LOOT ====
    if ((worldLoot || droppedC4Esp || liveNades || nadeTrajectory) &&
        g_masterEnabled) {
      for (int i = 0; i < slowLocal.itemCount; i++) {
        const WorldItem &it = slowLocal.items[i];
        if (!it.valid)
          continue;
        if (it.isC4) {
          if (!droppedC4Esp)
            continue;
          Vec2 scr;
          if (!WorldToScreen(it.position, liveVM, scr))
            continue;
          float distM = localOrigin.DistTo(it.position) * 0.0254f;
          char c4Buf[48];
          snprintf(c4Buf, sizeof(c4Buf), "[DROPPED C4] (%.0fm)", distM);
          ImVec2 tsz = ImGui::CalcTextSize(c4Buf);
          drawList->AddRectFilled({scr.x - tsz.x / 2.f - 6, scr.y - 4},
                                  {scr.x + tsz.x / 2.f + 6, scr.y + tsz.y + 4},
                                  IM_COL32(0, 0, 0, 200), 4.f);
          drawList->AddText({scr.x - tsz.x / 2.f + 1, scr.y + 1},
                            IM_COL32(0, 0, 0, 255), c4Buf);
          drawList->AddText({scr.x - tsz.x / 2.f, scr.y},
                            IM_COL32(190, 115, 255, 255), c4Buf);
          drawList->AddCircle({scr.x, scr.y + 18.f}, 6.f,
                              IM_COL32(190, 115, 255, 255), 12, 2.f);
          continue;
        }
        if (worldLoot && !it.isC4) {
          float dist = localOrigin.DistTo(it.position);
          if (dist > 3000.0f)
            continue;
          Vec2 scr;
          if (!WorldToScreen(it.position, liveVM, scr))
            continue;
          char lootBuf[64];
          snprintf(lootBuf, sizeof(lootBuf), "%s  %.0fm", it.name,
                   dist * 0.0254f);
          ImVec2 tsz =
              Inter_S_3 ? Inter_S_3->CalcTextSizeA(13.f, FLT_MAX, 0.0f, lootBuf)
                        : ImGui::CalcTextSize(lootBuf);
          float lx = scr.x - tsz.x * 0.5f;
          float ly = scr.y - tsz.y * 0.5f;
          drawList->AddRectFilled({lx - 6.0f, ly - 3.0f},
                                  {lx + tsz.x + 6.0f, ly + tsz.y + 3.0f},
                                  IM_COL32(18, 20, 28, 210), 4.0f);
          drawList->AddRect({lx - 6.0f, ly - 3.0f},
                            {lx + tsz.x + 6.0f, ly + tsz.y + 3.0f},
                            IM_COL32(168, 85, 247, 200), 4.0f, 0, 1.0f);
          if (Inter_S_3) {
            drawList->AddText(Inter_S_3, 13.f, {lx + 1.f, ly + 1.f},
                              IM_COL32(0, 0, 0, 220), lootBuf);
            drawList->AddText(Inter_S_3, 13.f, {lx, ly},
                              IM_COL32(235, 235, 245, 240), lootBuf);
          } else {
            drawList->AddText({lx + 1.f, ly + 1.f}, IM_COL32(0, 0, 0, 220),
                              lootBuf);
            drawList->AddText({lx, ly}, IM_COL32(235, 235, 245, 240), lootBuf);
          }
        }
      }
    }

    // ==== LIVE GRENADES ====
    if ((liveNades || nadeTrajectory) && g_masterEnabled) {
      for (int i = 0; i < slowLocal.projectileCount; i++) {
        const ProjectileItem &it = slowLocal.projectiles[i];
        if (!it.valid)
          continue;
        Vec2 scr;
        if (!WorldToScreen(it.position, liveVM, scr))
          continue;
        float distM = localOrigin.DistTo(it.position) * 0.0254f;
        ImU32 projCol =
            (it.projectileType == 2)   ? IM_COL32(0, 230, 255, 255)
            : (it.projectileType == 4) ? IM_COL32(255, 100, 0, 255)
            : (it.projectileType == 1) ? IM_COL32(255, 255, 120, 255)
            : (it.projectileType == 3) ? IM_COL32(255, 50, 50, 255)
                                       : IM_COL32(200, 200, 210, 255);
        float velMag = std::sqrt(it.velocity.x * it.velocity.x +
                                 it.velocity.y * it.velocity.y +
                                 it.velocity.z * it.velocity.z);
        char nadeBuf[64];
        if (it.projectileType == 3 && it.spawnTime > 0.0f &&
            slowLocal.bomb.curTime > 0.0f) {
          float elapsed = slowLocal.bomb.curTime - it.spawnTime;
          float fuseRem = std::max(0.0f, 1.65f - elapsed);
          snprintf(nadeBuf, sizeof(nadeBuf), "[%s] %.1fm | %.1fs", it.name,
                   distM, fuseRem);
        } else if (it.bounces > 0)
          snprintf(nadeBuf, sizeof(nadeBuf), "[%s] %.1fm (B:%d)", it.name,
                   distM, it.bounces);
        else
          snprintf(nadeBuf, sizeof(nadeBuf), "[%s] %.1fm", it.name, distM);
        ImVec2 tsz = ImGui::CalcTextSize(nadeBuf);
        float px = scr.x - tsz.x * 0.5f;
        float py = scr.y - 18.0f;
        drawList->AddRectFilled({px - 6.0f, py - 3.0f},
                                {px + tsz.x + 6.0f, py + tsz.y + 3.0f},
                                IM_COL32(12, 14, 20, 220), 4.0f);
        drawList->AddRect({px - 6.0f, py - 3.0f},
                          {px + tsz.x + 6.0f, py + tsz.y + 3.0f}, projCol, 4.0f,
                          0, 1.2f);
        drawList->AddText({px, py}, IM_COL32(255, 255, 255, 255), nadeBuf);
        drawList->AddCircleFilled({scr.x, scr.y}, 4.0f, projCol);
        drawList->AddCircle({scr.x, scr.y}, 8.0f, projCol, 16, 1.2f);
        if (nadeTrajectory) {
          Vec3 pLand = it.position;
          if (it.trailCount > 1) {
            Vec2 prevTrailScr;
            bool prevOk =
                WorldToScreen(it.trailPoints[0], liveVM, prevTrailScr);
            for (int t = 1; t < it.trailCount; t++) {
              Vec2 curTrailScr;
              bool curOk =
                  WorldToScreen(it.trailPoints[t], liveVM, curTrailScr);
              if (prevOk && curOk)
                drawList->AddLine({prevTrailScr.x, prevTrailScr.y},
                                  {curTrailScr.x, curTrailScr.y}, projCol,
                                  2.5f);
              prevTrailScr = curTrailScr;
              prevOk = curOk;
            }
            pLand = it.trailPoints[it.trailCount - 1];
          } else if (velMag > 15.0f) {
            constexpr float pDt = 1.0f / 64.0f;
            constexpr float pGrav = 320.0f;
            Vec3 pPos = it.position, pVel = it.velocity;
            Vec2 pPrevTr;
            bool pPrevTrOk = WorldToScreen(pPos, liveVM, pPrevTr);
            float groundZ =
                (it.bounces > 0) ? it.position.z : (localOrigin.z - 20.0f);
            int pBounces = it.bounces;
            for (int st = 0; st < 96; st++) {
              pVel.z -= pGrav * pDt;
              pVel.x *= 0.99f;
              pVel.y *= 0.99f;
              Vec3 pNext = {pPos.x + pVel.x * pDt, pPos.y + pVel.y * pDt,
                            pPos.z + pVel.z * pDt};
              if (pNext.z <= groundZ && pVel.z < 0.0f) {
                float frac = (pPos.z - groundZ) / (pPos.z - pNext.z + 0.0001f);
                if (frac >= 0.0f && frac <= 1.0f) {
                  Vec3 pBounce = {pPos.x + (pNext.x - pPos.x) * frac,
                                  pPos.y + (pNext.y - pPos.y) * frac, groundZ};
                  pLand = pBounce;
                  pPos = pBounce;
                  pBounces++;
                  pVel.z = -pVel.z * 0.45f;
                  pVel.x *= 0.65f;
                  pVel.y *= 0.65f;
                  if (it.projectileType == 4 || pBounces >= 3 ||
                      (pVel.x * pVel.x + pVel.y * pVel.y) < 1200.0f)
                    break;
                  continue;
                }
              }
              pPos = pNext;
              pLand = pPos;
              Vec2 pCurTr;
              bool pCurTrOk = WorldToScreen(pPos, liveVM, pCurTr);
              if (pPrevTrOk && pCurTrOk)
                drawList->AddLine({pPrevTr.x, pPrevTr.y}, {pCurTr.x, pCurTr.y},
                                  projCol, 2.2f);
              pPrevTr = pCurTr;
              pPrevTrOk = pCurTrOk;
            }
          }
          Vec2 pLandScr;
          if (WorldToScreen(pLand, liveVM, pLandScr)) {
            drawList->AddCircleFilled({pLandScr.x, pLandScr.y}, 4.0f, projCol);
            drawList->AddCircle({pLandScr.x, pLandScr.y}, 9.0f, projCol, 16,
                                1.8f);
          }
        }
        float hazardRadius = (it.projectileType == 3)   ? 180.0f
                             : (it.projectileType == 4) ? 160.0f
                             : (it.projectileType == 2) ? 144.0f
                                                        : 0.0f;
        if (hazardRadius > 0.0f) {
          static float kProjCos[16], kProjSin[16];
          static bool kProjInit = false;
          if (!kProjInit) {
            constexpr float kPStep = 2.0f * 3.14159265f / 16.0f;
            for (int p = 0; p < 16; p++) {
              kProjCos[p] = std::cos(p * kPStep);
              kProjSin[p] = std::sin(p * kPStep);
            }
            kProjInit = true;
          }
          Vec2 pPrev;
          bool pPrevOk = false;
          for (int p = 0; p <= 16; p++) {
            int pidx = p % 16;
            Vec3 pWp = {it.position.x + kProjCos[pidx] * hazardRadius,
                        it.position.y + kProjSin[pidx] * hazardRadius,
                        it.position.z};
            Vec2 pSp;
            bool pValid = WorldToScreen(pWp, liveVM, pSp);
            if (pValid && pPrevOk)
              drawList->AddLine({pPrev.x, pPrev.y}, {pSp.x, pSp.y}, projCol,
                                1.6f);
            pPrev = pSp;
            pPrevOk = pValid;
          }
          if (it.projectileType == 3 && distM < 6.5f) {
            char warnBuf[64];
            snprintf(warnBuf, sizeof(warnBuf),
                     "WARNING: INCOMING GRENADE (%.1fm)", distM);
            ImVec2 wTsz = ImGui::CalcTextSize(warnBuf);
            float wx = screenWidth * 0.5f - wTsz.x * 0.5f;
            float wy = screenHeight * 0.35f;
            drawList->AddRectFilled({wx - 10.0f, wy - 4.0f},
                                    {wx + wTsz.x + 10.0f, wy + wTsz.y + 4.0f},
                                    IM_COL32(200, 20, 20, 220), 4.0f);
            drawList->AddText({wx, wy}, IM_COL32(255, 255, 255, 255), warnBuf);
          }
        }
      }
    }

    // ==== GRENADE LINEUP HELPER ====
    if (nadeLineups && g_masterEnabled) {
      int activeSpotIdx = -1;
      float minActiveDist = 999999.0f;
      float curSec = (float)(GetTickCount64() % 10000000) * 0.001f;
      for (int i = 0; i < g_lineupCount; i++) {
        const auto &lu = g_lineups[i];
        if (lu.mapIdx != selectedMapIdx)
          continue;
        float dist = localOrigin.DistTo(lu.standPos);
        if (lineupShowAllMode == 0 && dist > 250.0f)
          continue;
        if (dist < 100.0f && dist < minActiveDist) {
          minActiveDist = dist;
          activeSpotIdx = i;
        }
        Vec2 standScr;
        if (WorldToScreen(lu.standPos, liveVM, standScr)) {
          ImU32 nadeThemeCol = (strcmp(lu.nadeType, "Smoke") == 0)
                                   ? IM_COL32(0, 230, 255, 240)
                               : (strcmp(lu.nadeType, "Molotov") == 0)
                                   ? IM_COL32(255, 120, 20, 240)
                               : (strcmp(lu.nadeType, "Flash") == 0)
                                   ? IM_COL32(195, 120, 255, 245)
                                   : IM_COL32(255, 60, 60, 240);
          float dist2D = std::sqrt((localOrigin.x - lu.standPos.x) *
                                       (localOrigin.x - lu.standPos.x) +
                                   (localOrigin.y - lu.standPos.y) *
                                       (localOrigin.y - lu.standPos.y));
          float dz = std::abs(localOrigin.z - lu.standPos.z);
          bool isStandingOnSpot = (dist2D < 42.0f && dz < 48.0f);
          float r = isStandingOnSpot ? 20.0f : (dist < 250.0f ? 14.0f : 9.0f);
          ImU32 groundCol =
              isStandingOnSpot ? IM_COL32(0, 255, 130, 255) : nadeThemeCol;
          Vec3 standTop = {lu.standPos.x, lu.standPos.y, lu.standPos.z + 64.0f};
          Vec2 standTopScr;
          if (WorldToScreen(standTop, liveVM, standTopScr)) {
            drawList->AddLine(
                {standScr.x, standScr.y}, {standTopScr.x, standTopScr.y},
                isStandingOnSpot ? IM_COL32(0, 255, 130, 160)
                                 : (nadeThemeCol & 0x00FFFFFF) | 0x60000000,
                2.0f);
          }
          drawList->AddCircleFilled({standScr.x, standScr.y}, r,
                                    IM_COL32(10, 14, 22, 190));
          drawList->AddCircle({standScr.x, standScr.y}, r, groundCol, 24,
                              isStandingOnSpot ? 2.8f : 1.5f);
          if (isStandingOnSpot) {
            float pulseR = r + 4.0f + 3.0f * std::sin(curSec * 7.0f);
            drawList->AddCircle({standScr.x, standScr.y}, pulseR,
                                IM_COL32(0, 255, 130, 120), 24, 1.2f);
          }
          char luBuf[96];
          if (isStandingOnSpot)
            snprintf(luBuf, sizeof(luBuf), "[STAND HERE] %s -> %s", lu.nadeType,
                     lu.title);
          else
            snprintf(luBuf, sizeof(luBuf), "[%s] %s (%.0fm)", lu.nadeType,
                     lu.title, dist * 0.0254f);
          ImVec2 tsz = ImGui::CalcTextSize(luBuf);
          float lx = standScr.x - tsz.x * 0.5f;
          float ly = standScr.y - r - tsz.y - 5.0f;
          drawList->AddRectFilled({lx - 5.0f, ly - 2.0f},
                                  {lx + tsz.x + 5.0f, ly + tsz.y + 2.0f},
                                  IM_COL32(10, 12, 18, 220), 4.0f);
          drawList->AddRect({lx - 5.0f, ly - 2.0f},
                            {lx + tsz.x + 5.0f, ly + tsz.y + 2.0f}, groundCol,
                            4.0f, 0, 1.0f);
          drawList->AddText({lx, ly},
                            isStandingOnSpot ? IM_COL32(0, 255, 130, 255)
                                             : IM_COL32(245, 248, 255, 255),
                            luBuf);
        }
      }
      if (activeSpotIdx >= 0) {
        const auto &lu = g_lineups[activeSpotIdx];
        constexpr float kDeg2Rad = 3.14159265358979323846f / 180.0f;
        float pRad = lu.targetPitch * kDeg2Rad;
        float yRad = lu.targetYaw * kDeg2Rad;
        float cosP = std::cos(pRad), sinP = std::sin(pRad);
        float cosY = std::cos(yRad), sinY = std::sin(yRad);
        Vec3 fwd = {cosP * cosY, cosP * sinY, -sinP};
        Vec3 eyePos = {lu.standPos.x, lu.standPos.y, lu.standPos.z + 64.0f};
        constexpr float kSkyDist = 1200.0f;
        Vec3 skyTarget = {eyePos.x + fwd.x * kSkyDist,
                          eyePos.y + fwd.y * kSkyDist,
                          eyePos.z + fwd.z * kSkyDist};
        Vec2 skyScr;
        bool inFront = WorldToScreen(skyTarget, liveVM, skyScr);
        float cx = screenWidth * 0.5f, cy = screenHeight * 0.5f;
        ImU32 themeCol =
            (strcmp(lu.nadeType, "Smoke") == 0) ? IM_COL32(0, 230, 255, 255)
            : (strcmp(lu.nadeType, "Molotov") == 0)
                ? IM_COL32(255, 120, 20, 255)
            : (strcmp(lu.nadeType, "Flash") == 0) ? IM_COL32(195, 120, 255, 255)
                                                  : IM_COL32(255, 60, 60, 255);
        if (inFront) {
          float dx = skyScr.x - cx, dy = skyScr.y - cy;
          float aimDist = std::sqrt(dx * dx + dy * dy);
          bool isAligned = (aimDist < 12.0f);
          ImU32 reticleCol = isAligned ? IM_COL32(0, 255, 120, 255) : themeCol;
          if (aimDist > 2.0f)
            drawList->AddLine({cx, cy}, {skyScr.x, skyScr.y},
                              isAligned ? IM_COL32(0, 255, 120, 220)
                                        : IM_COL32(255, 255, 255, 130),
                              isAligned ? 2.2f : 1.2f);
          float pulse = 1.0f + 0.12f * std::sin(curSec * 6.0f);
          float rOuter = (isAligned ? 14.0f : 11.0f) * pulse;
          float rInner = isAligned ? 4.5f : 2.5f;
          drawList->AddCircleFilled({skyScr.x, skyScr.y}, rOuter + 3.0f,
                                    IM_COL32(0, 0, 0, 150));
          drawList->AddCircle({skyScr.x, skyScr.y}, rOuter, reticleCol, 24,
                              isAligned ? 2.5f : 1.8f);
          drawList->AddCircleFilled({skyScr.x, skyScr.y}, rInner, reticleCol);
          float tLen = 6.0f;
          drawList->AddLine({skyScr.x - rOuter - tLen, skyScr.y},
                            {skyScr.x - rOuter + 1.0f, skyScr.y}, reticleCol,
                            1.5f);
          drawList->AddLine({skyScr.x + rOuter - 1.0f, skyScr.y},
                            {skyScr.x + rOuter + tLen, skyScr.y}, reticleCol,
                            1.5f);
          drawList->AddLine({skyScr.x, skyScr.y - rOuter - tLen},
                            {skyScr.x, skyScr.y - rOuter + 1.0f}, reticleCol,
                            1.5f);
          drawList->AddLine({skyScr.x, skyScr.y + rOuter - 1.0f},
                            {skyScr.x, skyScr.y + rOuter + tLen}, reticleCol,
                            1.5f);
          char skyBadge[128];
          if (isAligned)
            snprintf(skyBadge, sizeof(skyBadge), "[LOCKED - %s NOW!] -> %s",
                     lu.throwType == 1
                         ? "JUMPTHROW"
                         : (lu.throwType == 2 ? "RUN+THROW" : "THROW"),
                     lu.title);
          else
            snprintf(skyBadge, sizeof(skyBadge), "[AIM HERE IN SKY] %s (%s)",
                     lu.title,
                     lu.throwType == 1
                         ? "JUMPTHROW"
                         : (lu.throwType == 2 ? "RUN+THROW" : "STAND THROW"));
          ImVec2 sbTsz = ImGui::CalcTextSize(skyBadge);
          float sbX = skyScr.x - sbTsz.x * 0.5f;
          float sbY = skyScr.y - rOuter - sbTsz.y - 7.0f;
          drawList->AddRectFilled({sbX - 6.0f, sbY - 3.0f},
                                  {sbX + sbTsz.x + 6.0f, sbY + sbTsz.y + 3.0f},
                                  IM_COL32(10, 12, 18, 230), 4.0f);
          drawList->AddRect({sbX - 6.0f, sbY - 3.0f},
                            {sbX + sbTsz.x + 6.0f, sbY + sbTsz.y + 3.0f},
                            reticleCol, 4.0f, 0, 1.2f);
          drawList->AddText({sbX, sbY},
                            isAligned ? IM_COL32(0, 255, 120, 255)
                                      : IM_COL32(255, 255, 255, 240),
                            skyBadge);
          char angBuf[64];
          snprintf(angBuf, sizeof(angBuf), "Pitch: %.1f  Yaw: %.1f",
                   lu.targetPitch, lu.targetYaw);
          ImVec2 angTsz = ImGui::CalcTextSize(angBuf);
          float angX = skyScr.x - angTsz.x * 0.5f;
          float angY = skyScr.y + rOuter + 6.0f;
          drawList->AddRectFilled(
              {angX - 4.0f, angY - 1.0f},
              {angX + angTsz.x + 4.0f, angY + angTsz.y + 1.0f},
              IM_COL32(10, 12, 18, 190), 3.0f);
          drawList->AddText({angX, angY}, IM_COL32(190, 210, 235, 210), angBuf);
        } else {
          if (minActiveDist < 50.0f) {
            char lookUpBuf[96];
            snprintf(lookUpBuf, sizeof(lookUpBuf),
                     "^ LOOK UP INTO THE AIR -> %s (%s)", lu.title,
                     lu.throwType == 1 ? "JUMPTHROW" : "THROW");
            ImVec2 luTsz = ImGui::CalcTextSize(lookUpBuf);
            float lux = cx - luTsz.x * 0.5f;
            float luy = 45.0f;
            drawList->AddRectFilled(
                {lux - 8.0f, luy - 3.0f},
                {lux + luTsz.x + 8.0f, luy + luTsz.y + 3.0f},
                IM_COL32(15, 20, 30, 220), 4.0f);
            drawList->AddRect({lux - 8.0f, luy - 3.0f},
                              {lux + luTsz.x + 8.0f, luy + luTsz.y + 3.0f},
                              themeCol, 4.0f, 0, 1.2f);
            drawList->AddText({lux, luy}, themeCol, lookUpBuf);
          }
        }
      }
    }

    // ==== LOCAL GRENADE TRAJECTORY PREVIEW ====
    if (nadeTrajectory && g_masterEnabled) {
      bool isGrenadeEquipped =
          (snap.localWeaponDefIdx >= 43 && snap.localWeaponDefIdx <= 48);
      if (isGrenadeEquipped) {
        ImU32 trajCoreCol = IM_COL32(0, 230, 255, 255);
        ImU32 trajGlowCol = IM_COL32(0, 120, 200, 100);
        const char *nadeNameStr = "SMOKE";
        float blastRadius = 144.0f;
        switch (snap.localWeaponDefIdx) {
        case 43:
          trajCoreCol = IM_COL32(255, 255, 120, 255);
          trajGlowCol = IM_COL32(180, 180, 40, 90);
          nadeNameStr = "FLASH";
          blastRadius = 90.0f;
          break;
        case 44:
          trajCoreCol = IM_COL32(255, 55, 45, 255);
          trajGlowCol = IM_COL32(200, 30, 20, 90);
          nadeNameStr = "HE GRENADE";
          blastRadius = 180.0f;
          break;
        case 45:
          trajCoreCol = IM_COL32(0, 230, 255, 255);
          trajGlowCol = IM_COL32(0, 120, 200, 90);
          nadeNameStr = "SMOKE";
          blastRadius = 144.0f;
          break;
        case 46:
        case 48:
          trajCoreCol = IM_COL32(255, 120, 20, 255);
          trajGlowCol = IM_COL32(200, 60, 10, 90);
          nadeNameStr =
              (snap.localWeaponDefIdx == 46) ? "MOLOTOV" : "INCENDIARY";
          blastRadius = 160.0f;
          break;
        case 47:
          trajCoreCol = IM_COL32(200, 200, 210, 255);
          trajGlowCol = IM_COL32(80, 80, 90, 80);
          nadeNameStr = "DECOY";
          blastRadius = 60.0f;
          break;
        }
        Vec3 eyePos = localOrigin;
        if (snap.localViewOffset.z > 20.0f && snap.localViewOffset.z < 80.0f)
          eyePos.z += snap.localViewOffset.z;
        else
          eyePos.z += (snap.localFlags & 2) ? 46.0f : 64.0f;
        float rawPitch = snap.localViewAngles.x;
        float rawYaw = snap.localViewAngles.y;
        if (rawPitch == 0.0f && rawYaw == 0.0f) {
          Vec3 camFwd = {liveVM.m[3][0], liveVM.m[3][1], liveVM.m[3][2]};
          float len = std::sqrt(camFwd.x * camFwd.x + camFwd.y * camFwd.y +
                                camFwd.z * camFwd.z);
          if (len > 0.0001f) {
            camFwd.x /= len;
            camFwd.y /= len;
            camFwd.z /= len;
            rawPitch = -std::asin(camFwd.z) * 57.29577951308232f;
            rawYaw = std::atan2(camFwd.y, camFwd.x) * 57.29577951308232f;
          }
        }
        if (rawPitch < -89.0f)
          rawPitch = -89.0f;
        if (rawPitch > 89.0f)
          rawPitch = 89.0f;
        float throwPitch =
            rawPitch - (90.0f - std::abs(rawPitch)) * 10.0f / 90.0f;
        constexpr float kD2R = 3.14159265358979323846f / 180.0f;
        float pRad = throwPitch * kD2R;
        float yRad = rawYaw * kD2R;
        Vec3 fwd = {std::cos(pRad) * std::cos(yRad),
                    std::cos(pRad) * std::sin(yRad), -std::sin(pRad)};
        Vec3 startPos = {eyePos.x + fwd.x * 16.0f, eyePos.y + fwd.y * 16.0f,
                         eyePos.z + fwd.z * 16.0f - 2.0f};
        float rawStr = snap.localNadeThrowStrength;
        if (rawStr < 0.0f || rawStr > 1.0f)
          rawStr = 1.0f;
        float strength = (std::abs(rawStr - 0.5f) < 0.08f) ? 0.5f : rawStr;
        float baseVel =
            (snap.localWeaponDefIdx == 46 || snap.localWeaponDefIdx == 48)
                ? 700.0f
                : 750.0f;
        float throwSpeed = (strength * 0.7f + 0.3f) * (baseVel * 0.9f);
        throwSpeed = std::clamp(throwSpeed, 15.0f, 750.0f);
        Vec3 vel = {fwd.x * throwSpeed + snap.localVelocity.x * 1.25f,
                    fwd.y * throwSpeed + snap.localVelocity.y * 1.25f,
                    fwd.z * throwSpeed + snap.localVelocity.z * 1.25f};
        constexpr float dt3 = 1.0f / 64.0f;
        constexpr float gravity = 320.0f;
        constexpr float STOP_SPEED_SQ = 2036.0f;
        int maxSteps = 256;
        Vec3 trajPts[258];
        trajPts[0] = startPos;
        int validSteps = 1;
        Vec3 curPos = startPos, curVel = vel;
        // OPT: groundRef fallback logic — use eye Z minus a fixed offset
        // (better than raw local Z)
        float groundRef = eyePos.z - 64.0f;
        Vec3 impactPos = startPos;
        bool hasImpact = false;
        int bounceCount = 0;
        for (int step = 0; step < maxSteps; step++) {
          float newVz = curVel.z - gravity * dt3;
          Vec3 moveVec = {curVel.x * dt3, curVel.y * dt3,
                          (curVel.z + newVz) * 0.5f * dt3};
          curVel.z = newVz;
          Vec3 nextPos = {curPos.x + moveVec.x, curPos.y + moveVec.y,
                          curPos.z + moveVec.z};
          if (nextPos.z <= groundRef && curVel.z < 0.0f) {
            float frac =
                (curPos.z - groundRef) / (curPos.z - nextPos.z + 0.0001f);
            if (frac >= 0.0f && frac <= 1.0f) {
              Vec3 hitPt = {curPos.x + (nextPos.x - curPos.x) * frac,
                            curPos.y + (nextPos.y - curPos.y) * frac,
                            groundRef};
              trajPts[validSteps++] = hitPt;
              if (snap.localWeaponDefIdx == 46 ||
                  snap.localWeaponDefIdx == 48) {
                impactPos = hitPt;
                hasImpact = true;
                break;
              }
              bounceCount++;
              curVel.z = -curVel.z * 0.45f;
              curVel.x *= 0.65f;
              curVel.y *= 0.65f;
              curPos = hitPt;
              float speedSq = curVel.x * curVel.x + curVel.y * curVel.y +
                              curVel.z * curVel.z;
              if (speedSq < STOP_SPEED_SQ || bounceCount >= 6) {
                impactPos = hitPt;
                hasImpact = true;
                break;
              }
              continue;
            }
          }
          curPos = nextPos;
          trajPts[validSteps++] = curPos;
          if ((snap.localWeaponDefIdx == 43 || snap.localWeaponDefIdx == 44) &&
              ((step - 8) * dt3 > 1.50f)) {
            impactPos = curPos;
            hasImpact = true;
            break;
          }
        }
        if (!hasImpact)
          impactPos = trajPts[validSteps - 1];
        Vec2 prevScr;
        bool prevValid = WorldToScreen(trajPts[0], liveVM, prevScr);
        for (int p = 1; p < validSteps; p++) {
          Vec2 nextScr;
          bool nextValid = WorldToScreen(trajPts[p], liveVM, nextScr);
          if (prevValid && nextValid) {
            drawList->AddLine({prevScr.x, prevScr.y}, {nextScr.x, nextScr.y},
                              trajGlowCol, 3.8f);
            drawList->AddLine({prevScr.x, prevScr.y}, {nextScr.x, nextScr.y},
                              trajCoreCol, 1.8f);
            if (p % 8 == 0)
              drawList->AddCircleFilled({nextScr.x, nextScr.y}, 2.5f,
                                        trajCoreCol);
          }
          prevScr = nextScr;
          prevValid = nextValid;
        }
        Vec2 impactScr;
        if (WorldToScreen(impactPos, liveVM, impactScr)) {
          float curSec2 = (float)(GetTickCount64() % 10000000) * 0.001f;
          float pulseR = 9.0f + 3.0f * std::sin(curSec2 * 8.0f);
          drawList->AddCircleFilled({impactScr.x, impactScr.y}, 4.0f,
                                    trajCoreCol);
          drawList->AddCircle({impactScr.x, impactScr.y}, pulseR, trajCoreCol,
                              24, 1.8f);
          drawList->AddCircle({impactScr.x, impactScr.y}, pulseR + 4.0f,
                              trajGlowCol, 24, 1.0f);
          float tLen = 6.0f;
          drawList->AddLine({impactScr.x - pulseR - tLen, impactScr.y},
                            {impactScr.x - pulseR + 1.0f, impactScr.y},
                            trajCoreCol, 1.5f);
          drawList->AddLine({impactScr.x + pulseR - 1.0f, impactScr.y},
                            {impactScr.x + pulseR + tLen, impactScr.y},
                            trajCoreCol, 1.5f);
          drawList->AddLine({impactScr.x, impactScr.y - pulseR - tLen},
                            {impactScr.x, impactScr.y - pulseR + 1.0f},
                            trajCoreCol, 1.5f);
          drawList->AddLine({impactScr.x, impactScr.y + pulseR - 1.0f},
                            {impactScr.x, impactScr.y + pulseR + tLen},
                            trajCoreCol, 1.5f);
          if (blastRadius > 0.0f) {
            static float s_cos16[16], s_sin16[16];
            static bool s_init16 = false;
            if (!s_init16) {
              constexpr float kStep = 2.0f * 3.14159265f / 16.0f;
              for (int i = 0; i < 16; i++) {
                s_cos16[i] = std::cos(i * kStep);
                s_sin16[i] = std::sin(i * kStep);
              }
              s_init16 = true;
            }
            Vec2 ringPrev;
            bool ringPrevOk = false;
            for (int i = 0; i <= 16; i++) {
              int idx = i % 16;
              Vec3 ringPt = {impactPos.x + s_cos16[idx] * blastRadius,
                             impactPos.y + s_sin16[idx] * blastRadius,
                             impactPos.z + 1.0f};
              Vec2 ringScr;
              bool ringOk = WorldToScreen(ringPt, liveVM, ringScr);
              if (ringOk && ringPrevOk)
                drawList->AddLine({ringPrev.x, ringPrev.y},
                                  {ringScr.x, ringScr.y}, trajGlowCol, 1.6f);
              ringPrev = ringScr;
              ringPrevOk = ringOk;
            }
          }
          float distM = localOrigin.DistTo(impactPos) * 0.0254f;
          char impBuf[64];
          snprintf(impBuf, sizeof(impBuf), "[%s] %.1fm", nadeNameStr, distM);
          ImVec2 ibTsz = ImGui::CalcTextSize(impBuf);
          float ibX = impactScr.x - ibTsz.x * 0.5f;
          float ibY = impactScr.y - pulseR - ibTsz.y - 6.0f;
          drawList->AddRectFilled({ibX - 6.0f, ibY - 2.0f},
                                  {ibX + ibTsz.x + 6.0f, ibY + ibTsz.y + 2.0f},
                                  IM_COL32(10, 14, 22, 225), 4.0f);
          drawList->AddRect({ibX - 6.0f, ibY - 2.0f},
                            {ibX + ibTsz.x + 6.0f, ibY + ibTsz.y + 2.0f},
                            trajCoreCol, 4.0f, 0, 1.2f);
          drawList->AddText({ibX, ibY}, IM_COL32(255, 255, 255, 255), impBuf);
        }
        if (snap.localNadePinPulled) {
          char pinBuf[64];
          const char *throwMode = (strength >= 0.85f)  ? "FULL THROW"
                                  : (strength <= 0.2f) ? "UNDERHAND LOB"
                                                       : "MEDIUM TOSS";
          snprintf(pinBuf, sizeof(pinBuf), "[PIN PULLED] %s (%.0f%%)",
                   throwMode, strength * 100.0f);
          ImVec2 pTsz = ImGui::CalcTextSize(pinBuf);
          float px = screenWidth * 0.5f - pTsz.x * 0.5f;
          float py = screenHeight * 0.55f;
          drawList->AddRectFilled({px - 8.0f, py - 4.0f},
                                  {px + pTsz.x + 8.0f, py + pTsz.y + 12.0f},
                                  IM_COL32(10, 14, 22, 220), 4.0f);
          drawList->AddRect({px - 8.0f, py - 4.0f},
                            {px + pTsz.x + 8.0f, py + pTsz.y + 12.0f},
                            trajCoreCol, 4.0f, 0, 1.2f);
          drawList->AddText({px, py}, IM_COL32(255, 255, 255, 255), pinBuf);
          float sBarW = pTsz.x;
          float sBarY = py + pTsz.y + 3.0f;
          drawList->AddRectFilled({px, sBarY}, {px + sBarW, sBarY + 3.0f},
                                  IM_COL32(40, 44, 55, 200), 1.5f);
          drawList->AddRectFilled({px, sBarY},
                                  {px + sBarW * strength, sBarY + 3.0f},
                                  trajCoreCol, 1.5f);
        }
      }
    }

    // ==== BULLET TRACERS ====
    struct TracerPoint {
      Vec3 start;
      Vec3 end;
      std::chrono::steady_clock::time_point time;
      bool active = false;
    };
    static TracerPoint s_tracerRing[32];
    static int s_tracerHead = 0;
    static int s_prevLocalShots = 0;
    if (snap.localShotsFired > s_prevLocalShots && snap.localShotsFired > 0) {
      Vec3 camFwd = {liveVM.m[2][0], liveVM.m[2][1], liveVM.m[2][2]};
      Vec3 eyePos = {localOrigin.x, localOrigin.y, localOrigin.z + 64.0f};
      Vec3 hitPos = {eyePos.x + camFwd.x * 2400.0f,
                     eyePos.y + camFwd.y * 2400.0f,
                     eyePos.z + camFwd.z * 2400.0f};
      auto &slot = s_tracerRing[s_tracerHead];
      slot.start = eyePos;
      slot.end = hitPos;
      slot.time = std::chrono::steady_clock::now();
      slot.active = true;
      s_tracerHead = (s_tracerHead + 1) & 31;
    }
    s_prevLocalShots = snap.localShotsFired;
    if (bulletTracer && g_masterEnabled) {
      auto now = std::chrono::steady_clock::now();
      const uint8_t trR = (uint8_t)(bulletColor[0] * 255.f);
      const uint8_t trG = (uint8_t)(bulletColor[1] * 255.f);
      const uint8_t trB = (uint8_t)(bulletColor[2] * 255.f);
      for (int ti = 0; ti < 32; ti++) {
        auto &tr = s_tracerRing[ti];
        if (!tr.active)
          continue;
        float el = std::chrono::duration<float>(now - tr.time).count();
        if (el > 0.65f) {
          tr.active = false;
          continue;
        }
        Vec2 p1, p2;
        if (WorldToScreen(tr.start, liveVM, p1) &&
            WorldToScreen(tr.end, liveVM, p2)) {
          uint8_t a = (uint8_t)((1.0f - el * 1.5384f) * 220.0f);
          drawList->AddLine({p1.x, p1.y}, {p2.x, p2.y},
                            IM_COL32(trR, trG, trB, a), 1.8f);
        }
      }
    }

    // ==== NEWGUI HUD OVERLAYS ====
    if (g_masterEnabled) {
      float localSpeed = std::sqrt(snap.localVelocity.x * snap.localVelocity.x + snap.localVelocity.y * snap.localVelocity.y);
      bool inAir = !(snap.localFlags & 1);
      HudGui::RenderOverlays(slowLocal.spectatorCount,
                             (const char (*)[64])slowLocal.spectators,
                             menuOpen,
                             localSpeed,
                             inAir);
    }
    if (g_Config.island.enabled)
      DynamicIsland::Render();
    if (menuOpen)
      RenderMainWindow();

    ImGui::Render();
    g_snapRenderReading.store(-1, std::memory_order_release);
    const float clr[4] = {0, 0, 0, 0};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(0, 0);

    static HighPrecisionSleep s_precTimer;
    int activeMaxFps = std::clamp(g_Config.max_fps, 30, 500);
    if (activeMaxFps < 360) {
      long long frameMicros = 1000000LL / activeMaxFps;
      auto now2 = std::chrono::high_resolution_clock::now();
      if (nextFrameTime < now2)
        nextFrameTime = now2;
      nextFrameTime += std::chrono::microseconds(frameMicros);
      now2 = std::chrono::high_resolution_clock::now();
      if (now2 < nextFrameTime) {
        long long remainingUs =
            std::chrono::duration_cast<std::chrono::microseconds>(
                nextFrameTime - now2)
                .count();
        if (remainingUs > 1800)
          s_precTimer.SleepMicros(remainingUs - 1200);
        while (std::chrono::high_resolution_clock::now() < nextFrameTime)
          _mm_pause();
      }
    }
  }

  running = false;
  if (autoSaveConfig)
    SaveConfig("config.cfg");
  if (memThread.joinable())
    memThread.join();
  if (slowThread.joinable())
    slowThread.join();
  DeleteCriticalSection(&g_snapLock);
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  CleanupDeviceD3D();
  ::DestroyWindow(hwndOverlay);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
  if (hGame)
    CloseHandle(hGame);
  return 0;
}