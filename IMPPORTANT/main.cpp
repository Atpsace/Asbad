// main.cpp — CS2 ESP  |  Adapted from CSGO_Public-main feature set
// Build 14178 verified offsets  |  Extended: aimbot, triggerbot, bomb, loot, sound, spectators
#include <windows.h>
#include <tlhelp32.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <algorithm>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "Font.h"
#include "image.h"
#include "custom_colors.h"
#include "custom_settings.h"
#include "imgui_settings.h"
#include "custom_widgets.h"
#include "main.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ============ CS2 OFFSETS (Build 14185 from sezzyaep/CS2-OFFSETS) ============
namespace O {
    // --- Verified offsets ---
    constexpr uintptr_t dwEntityList     = 0x27151A8;
    constexpr uintptr_t dwLocalPlayerPawn= 0x25606D8;
    constexpr uintptr_t dwViewMatrix     = 0x2565A20;

    constexpr uintptr_t m_hPlayerPawn    = 0x92C;
    constexpr uintptr_t m_bPawnIsAlive   = 0x934;
    constexpr uintptr_t m_iszPlayerName  = 0x6FC;
    constexpr uintptr_t m_iHealth        = 0x34C;
    constexpr uintptr_t m_iTeamNum       = 0x3E7;
    constexpr uintptr_t m_pGameSceneNode = 0x330;
    constexpr uintptr_t m_modelState     = 0x140;
    constexpr uintptr_t m_boneArray      = 0x1C0;
    constexpr uintptr_t m_vecAbsOrigin   = 0xC8;
    constexpr uintptr_t m_vOldOrigin     = 0x14A4;

    // --- Schema additions ---
    constexpr uintptr_t m_pWeaponSvc     = 0x12F0;  // CPlayer_WeaponServices* on pawn
    constexpr uintptr_t m_hActiveWpn     = 0x60;    // CHandle<weapon> inside weapon svc
    constexpr uintptr_t m_AttrMgr        = 0x1290;  // C_AttributeContainer on weapon entity
    constexpr uintptr_t m_Item_off       = 0x50;    // C_EconItemView inside attr mgr
    constexpr uintptr_t m_iItemDefIdx    = 0x1BA;   // uint16_t item definition index
    constexpr uintptr_t m_pEntity_id     = 0x10;    // CEntityInstance::m_pEntity (identity ptr)
    constexpr uintptr_t m_designerName   = 0x20;    // CEntityIdentity::m_designerName (char*)
    constexpr uintptr_t m_pObserverSvc   = 0x1308;  // CPlayer_ObserverServices* on pawn
    constexpr uintptr_t m_hObsTgt        = 0x4C;    // m_hObserverTarget inside observer svc
    constexpr uintptr_t m_emitSndTime    = 0x1EA8;  // float m_flEmitSoundTime on pawn
    constexpr uintptr_t m_absVelocity    = 0x3F8;   // Vec3 m_vecAbsVelocity on entity
    constexpr uintptr_t m_iShotsFired    = 0x1EB4;  // int m_iShotsFired on pawn
    constexpr uintptr_t m_angEyeAngles   = 0x35F0;  // Vec2 m_angEyeAngles (pitch,yaw) on pawn
    constexpr uintptr_t m_ArmorValue     = 0x1ECC;  // int m_ArmorValue on C_CSPlayerPawn

    // Planted C4
    constexpr uintptr_t dwPlantedC4      = 0x24C9290;
    constexpr uintptr_t c4_bTicking      = 0x1288;
    constexpr uintptr_t c4_nSite         = 0x128C;
    constexpr uintptr_t c4_flBlow        = 0x12B8;
    constexpr uintptr_t c4_flTimer       = 0x12C0;
    constexpr uintptr_t c4_bDefusing     = 0x12C4;
    constexpr uintptr_t c4_flDefuseLen   = 0x12D4;
    constexpr uintptr_t c4_flDefuseCountDown = 0x12D8;
    constexpr uintptr_t c4_bDefused      = 0x12DC;
} // namespace O

// ============ TIMER RESOLUTION GUARD (from CSGO_Public-main Main.cpp) ============
// Raises Windows scheduler quantum to 1 ms for the lifetime of the object.
// Without this, Sleep(1) rounds up to ~15.6 ms — capping loops to ~64 Hz.
struct TimerGuard {
    bool ok = false;
    TimerGuard()  { ok = (timeBeginPeriod(1) == TIMERR_NOERROR); }
    ~TimerGuard() { if (ok) timeEndPeriod(1); }
};

// ============ PROCESS GLOBALS ============
HANDLE hGame     = NULL;
DWORD  pid       = 0;
uintptr_t clientBase = 0;

// ============ FEATURE FLAGS ============
bool  menuOpen      = false;
bool  g_masterEnabled = true;   // Panic toggle — HOME key
bool  espEnabled    = true;
bool  drawBoxes     = true;
int   boxStyle      = 0;
bool  drawSkeletons = true;
int   skeletonStyle = 0; // 0 = Optimized (Stick), 1 = Full Skeleton
bool  drawHeadDot   = true;
bool  drawHealth    = true;
bool  drawArmor     = true;
bool  drawNames     = true;
bool  drawDistance  = true;
bool  drawSnaplines = false;
bool  drawWeapon    = true;
bool  drawEyeRay    = false;
bool  teamCheck     = true;
int   colorPalette  = 0;

// Sound ESP
bool  soundEsp      = true;

// Bomb ESP
bool  bombEsp       = true;

// Spectator list
bool  showSpectators = true;

// World loot
bool  worldLoot     = false;

// ============ AIMBOT CONFIG ============
bool  aimbotEnabled   = false;
int   aimbotKey       = 0;      // 0=LAlt 1=RAlt 2=LShift 3=RShift
float aimbotFovDeg    = 5.0f;
float aimbotSmooth    = 4.0f;
int   aimbotBone      = 0;      // 0=head(7) 1=neck(6) 2=chest(4)
bool  aimbotFovCircle = true;
bool  aimbotLead      = false;
bool  aimbotWallCheck = false;  // (visibility gate via spotted — optional)
bool  bulletTracer    = false;
float bulletColor[4]  = { 0.0f, 0.584f, 1.0f, 1.0f };
int   radarDistance   = 60;
bool  chamsEnabled    = false;
int   chamsType       = 0;
float chamsColor[4]   = { 1.0f, 0.0f, 0.0f, 1.0f };
int   chamsDistance   = 60;

// ============ TRIGGERBOT CONFIG ============
bool  triggerbotEnabled = false;
int   triggerMode       = 0;    // 0=key-hold 1=always
int   triggerDelay      = 40;   // ms

// ============ DX11 GLOBALS ============
static ID3D11Device*            g_pd3dDevice          = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext    = NULL;
static IDXGISwapChain*          g_pSwapChain           = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
static ID3D11BlendState*        g_pBlendState          = NULL;

HWND hwndOverlay  = NULL;
int  screenWidth  = 1920;
int  screenHeight = 1080;

namespace image {
    ID3D11ShaderResourceView* settings_checkbox = nullptr;
    ID3D11ShaderResourceView* user   = nullptr;
    ID3D11ShaderResourceView* fps    = nullptr;
    ID3D11ShaderResourceView* ms     = nullptr;
    ID3D11ShaderResourceView* logo   = nullptr;
}

// ============ THREAD SYNC & TELEMETRY ============
std::atomic<bool>  running{true};
std::atomic<int>   telemetryFps{0};
std::atomic<int>   telemetryTargets{0};
std::atomic<float> telemetryScanMs{0.07f};

// Per-entity slot: wall-clock ms when m_flEmitSoundTime last changed (0 = never)
std::atomic<uint64_t> g_soundWallMs[64]{};

// ============ MATH & VECTOR STRUCTS ============
struct Vec2 { float x = 0, y = 0; };
struct Vec3 {
    float x = 0, y = 0, z = 0;
    float DistTo(const Vec3& o) const {
        float dx = x-o.x, dy = y-o.y, dz = z-o.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator*(float s)        const { return {x*s,   y*s,   z*s};   }
};
struct Matrix4x4 { float m[4][4]; };
struct BoneJointData { Vec3 pos; char pad[0x14]; }; // 32-byte CS2 bone joint

// ============ WEAPON NAME LOOKUP (from CSGO_Public-main / base/Entity.cpp) ============
static const char* WeaponNameFromDefIdx(uint16_t idx) {
    switch (idx) {
        case 1:  return "Deagle";     case 2:  return "Elites";
        case 3:  return "FiveSeven";  case 4:  return "Glock";
        case 7:  return "AK-47";      case 8:  return "AUG";
        case 9:  return "AWP";        case 10: return "FAMAS";
        case 11: return "G3SG1";      case 13: return "Galil";
        case 14: return "M249";       case 16: return "M4A1";
        case 17: return "MAC-10";     case 19: return "P90";
        case 23: return "MP5-SD";     case 24: return "UMP-45";
        case 25: return "XM1014";     case 26: return "PP-Bizon";
        case 27: return "MAG-7";      case 28: return "Negev";
        case 29: return "Sawed-Off";  case 30: return "Tec-9";
        case 31: return "Zeus";       case 32: return "P2000";
        case 33: return "MP7";        case 34: return "MP9";
        case 35: return "Nova";       case 36: return "P250";
        case 38: return "SCAR-20";    case 39: return "SG553";
        case 40: return "SSG08";      case 42: return "Knife";
        case 43: return "Flash";      case 44: return "HE";
        case 45: return "Smoke";      case 46: return "Molotov";
        case 47: return "Decoy";      case 48: return "Incen";
        case 49: return "C4";         case 60: return "M4A1-S";
        case 61: return "USP-S";      case 63: return "CZ75";
        case 64: return "R8";
        case 500: case 503: case 505: case 506: case 507: case 508:
        case 509: case 512: case 514: case 515: case 516: case 519:
        case 520: case 522: case 523: case 525: return "Knife";
        default: return nullptr;
    }
}

// ============ ENTITY / SNAPSHOT STRUCTS ============
struct EntityData {
    bool  valid      = false;
    int   health     = 0;
    int   armor      = 0;
    int   team       = 0;
    int   shotsFired = 0;
    int   entitySlot = -1;      // index into CachedTarget / g_soundWallMs
    char  name[64]   = {0};
    char  weaponName[32] = {0};
    Vec3  origin{};
    Vec3  headPos{};
    Vec3  velocity{};
    Vec2  eyeAngles{};           // pitch, yaw
    float lastSoundTime = 0.0f;
    bool  hasBones  = false;
    Vec3  bones[32]{};
};

struct BombState {
    bool  valid      = false;
    bool  ticking    = false;
    bool  defusing   = false;
    bool  defused    = false;
    int   site       = -1;        // 0=A, 1=B
    float timerLength  = 40.f;
    float defuseLength = 10.f;
    Vec3  worldPos{};
    std::chrono::steady_clock::time_point plantWallTime;
    bool  wallTimeCaptured = false;
};

struct WorldItem {
    bool  valid = false;
    char  name[40] = {0};
    Vec3  position{};
};

struct SpectatorEntry {
    char  name[64] = {0};
};

struct FrameSnapshot {
    Matrix4x4     viewMatrix{};
    Vec3          localOrigin{};
    int           localTeam = 0;
    int           count     = 0;
    EntityData    entities[64]{};
};

// Slow-scan data (bomb, loot, spectators) — updated every ~500 ms
struct SlowSnapshot {
    BombState     bomb{};
    WorldItem     items[64]{};
    int           itemCount = 0;
    SpectatorEntry spectators[8]{};
    int           spectatorCount = 0;
};

FrameSnapshot g_backSnapshot;
CRITICAL_SECTION g_snapLock;

SlowSnapshot g_slowSnap;
CRITICAL_SECTION g_slowLock;

// ============ CACHED TARGET (per entity slot) ============
struct CachedTarget {
    bool      active         = false;
    uint32_t  pawnHandle     = 0;
    uintptr_t pawn           = 0;
    uintptr_t gameSceneNode  = 0;
    uintptr_t boneArrayAddress = 0;
    int       team           = 0;
    char      name[64]       = {0};
    char      weaponName[32] = {0};
    float     cachedSoundTime = 0.0f;
};

// ============ WORLD-TO-SCREEN ============
bool WorldToScreen(const Vec3& world, const Matrix4x4& vm, Vec2& screen) {
    float clipW = world.x*vm.m[3][0] + world.y*vm.m[3][1] + world.z*vm.m[3][2] + vm.m[3][3];
    if (clipW < 0.001f) return false;
    float clipX = world.x*vm.m[0][0] + world.y*vm.m[0][1] + world.z*vm.m[0][2] + vm.m[0][3];
    float clipY = world.x*vm.m[1][0] + world.y*vm.m[1][1] + world.z*vm.m[1][2] + vm.m[1][3];
    float ndcX  = clipX / clipW;
    float ndcY  = clipY / clipW;
    screen.x = (screenWidth  / 2.0f) + (ndcX * screenWidth  / 2.0f);
    screen.y = (screenHeight / 2.0f) - (ndcY * screenHeight / 2.0f);
    return true;
}

// ============ DX11 SRV HELPER ============
ID3D11ShaderResourceView* CreateSRVFromMemoryDX11(ID3D11Device* dev, const unsigned char* data, size_t sz) {
    if (!dev || !data || sz == 0) return nullptr;
    int w = 0, h = 0, comp = 0;
    unsigned char* img = stbi_load_from_memory(data, (int)sz, &w, &h, &comp, 4);
    if (!img) return nullptr;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = w; desc.Height = h; desc.MipLevels = 1; desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SUBRESOURCE_DATA init = {}; init.pSysMem = img; init.SysMemPitch = w * 4;
    ID3D11Texture2D* tex = nullptr;
    HRESULT hr = dev->CreateTexture2D(&desc, &init, &tex);
    stbi_image_free(img);
    if (FAILED(hr) || !tex) return nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    ID3D11ShaderResourceView* srv = nullptr;
    hr = dev->CreateShaderResourceView(tex, &srvDesc, &srv);
    tex->Release();
    return srv;
}

// ============ MEMORY WORKER THREAD ============
void MemoryWorkerThread() {
    CachedTarget targets[64]{};
    auto lastCacheTime = std::chrono::steady_clock::now() - std::chrono::seconds(10);

    while (running) {
        auto tStart = std::chrono::high_resolution_clock::now();

        // Dynamic attach if CS2 restarted
        if (!hGame || !clientBase) {
            HWND hwndCS2 = FindWindowA("SDL_app", "Counter-Strike 2");
            if (!hwndCS2) hwndCS2 = FindWindowA(NULL, "Counter-Strike 2");
            if (hwndCS2) {
                GetWindowThreadProcessId(hwndCS2, &pid);
                if (pid) {
                    if (!hGame) hGame = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
                    if (hGame && !clientBase) {
                        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
                        if (hSnap != INVALID_HANDLE_VALUE) {
                            MODULEENTRY32W me = {sizeof(me)};
                            if (Module32FirstW(hSnap, &me)) {
                                do {
                                    if (wcsicmp(me.szModule, L"client.dll") == 0) {
                                        clientBase = reinterpret_cast<uintptr_t>(me.modBaseAddr);
                                        break;
                                    }
                                } while (Module32NextW(hSnap, &me));
                            }
                            CloseHandle(hSnap);
                        }
                    }
                }
            }
            if (!hGame || !clientBase) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
        }

        FrameSnapshot snap{};

        // View matrix
        ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwViewMatrix), &snap.viewMatrix, sizeof(snap.viewMatrix), nullptr);

        // Local player
        uintptr_t localPawn = 0;
        ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwLocalPlayerPawn), &localPawn, sizeof(localPawn), nullptr);
        if (localPawn) {
            ReadProcessMemory(hGame, (LPCVOID)(localPawn + O::m_iTeamNum),   &snap.localTeam,   sizeof(snap.localTeam),   nullptr);
            ReadProcessMemory(hGame, (LPCVOID)(localPawn + O::m_vOldOrigin), &snap.localOrigin, sizeof(snap.localOrigin), nullptr);
        }

        uintptr_t entityList = 0;
        ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList), &entityList, sizeof(entityList), nullptr);

        if (entityList) {
            auto now = std::chrono::steady_clock::now();
            bool doCache = (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCacheTime).count() >= 200);
            if (doCache) {
                lastCacheTime = now;

                uintptr_t listEntry = 0;
                ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10), &listEntry, sizeof(listEntry), nullptr);
                if (listEntry) {
                    BYTE controllerBlock[0x2000];
                    if (ReadProcessMemory(hGame, (LPCVOID)listEntry, controllerBlock, sizeof(controllerBlock), nullptr)) {
                        for (int i = 1; i < 64; i++) {
                            uintptr_t controller = *reinterpret_cast<uintptr_t*>(controllerBlock + (i * 0x70));
                            if (!controller && (i * 0x78 + 8 <= (int)sizeof(controllerBlock)))
                                controller = *reinterpret_cast<uintptr_t*>(controllerBlock + (i * 0x78));
                            if (!controller) { targets[i].active = false; continue; }

                            uint32_t pawnHandle = 0;
                            ReadProcessMemory(hGame, (LPCVOID)(controller + O::m_hPlayerPawn), &pawnHandle, sizeof(pawnHandle), nullptr);
                            if (!pawnHandle) { targets[i].active = false; continue; }

                            // Retain cache if pawn + bones still valid
                            if (targets[i].active && targets[i].pawnHandle == pawnHandle &&
                                targets[i].pawn && targets[i].boneArrayAddress) continue;

                            // Resolve pawn from handle
                            uintptr_t listEntry2 = 0;
                            uintptr_t pawnEntryOff = entityList + 0x10 + 8 * ((pawnHandle & 0x7FFF) >> 9);
                            ReadProcessMemory(hGame, (LPCVOID)pawnEntryOff, &listEntry2, sizeof(listEntry2), nullptr);
                            if (!listEntry2) { targets[i].active = false; continue; }

                            uintptr_t pawn = 0;
                            ReadProcessMemory(hGame, (LPCVOID)(listEntry2 + 0x70 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
                            if (!pawn) ReadProcessMemory(hGame, (LPCVOID)(listEntry2 + 0x78 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
                            if (!pawn) { targets[i].active = false; continue; }

                            targets[i].pawn       = pawn;
                            targets[i].pawnHandle = pawnHandle;

                            // Team
                            ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_iTeamNum), &targets[i].team, sizeof(targets[i].team), nullptr);

                            // Name
                            char nameBuf[64] = {0};
                            ReadProcessMemory(hGame, (LPCVOID)(controller + O::m_iszPlayerName), nameBuf, sizeof(nameBuf)-1, nullptr);
                            strncpy(targets[i].name, (nameBuf[0] != '\0') ? nameBuf : "Enemy", sizeof(targets[i].name)-1);
                            targets[i].name[sizeof(targets[i].name)-1] = '\0';

                            // GameSceneNode + BoneArray
                            uintptr_t gsn = 0;
                            ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pGameSceneNode), &gsn, sizeof(gsn), nullptr);
                            targets[i].gameSceneNode = gsn;
                            uintptr_t boneArr = 0;
                            if (gsn) ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_boneArray), &boneArr, sizeof(boneArr), nullptr);
                            targets[i].boneArrayAddress = boneArr;
                            targets[i].active = true;

                            // Weapon name (per CSGO_Public-main entity_update — item def index lookup)
                            {
                                uintptr_t wpnSvc = 0;
                                ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pWeaponSvc), &wpnSvc, sizeof(wpnSvc), nullptr);
                                if (wpnSvc) {
                                    uint32_t activeWpnH = 0;
                                    ReadProcessMemory(hGame, (LPCVOID)(wpnSvc + O::m_hActiveWpn), &activeWpnH, sizeof(activeWpnH), nullptr);
                                    if (activeWpnH && activeWpnH != 0xFFFFFFFF) {
                                        uintptr_t wpnListEntry = 0;
                                        ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10 + 8 * ((activeWpnH & 0x7FFF) >> 9)), &wpnListEntry, sizeof(wpnListEntry), nullptr);
                                        if (wpnListEntry) {
                                            uintptr_t wpnEnt = 0;
                                            ReadProcessMemory(hGame, (LPCVOID)(wpnListEntry + 0x70 * (activeWpnH & 0x1FF)), &wpnEnt, sizeof(wpnEnt), nullptr);
                                            if (!wpnEnt) ReadProcessMemory(hGame, (LPCVOID)(wpnListEntry + 0x78 * (activeWpnH & 0x1FF)), &wpnEnt, sizeof(wpnEnt), nullptr);
                                            if (wpnEnt) {
                                                uint16_t defIdx = 0;
                                                ReadProcessMemory(hGame, (LPCVOID)(wpnEnt + O::m_AttrMgr + O::m_Item_off + O::m_iItemDefIdx), &defIdx, sizeof(defIdx), nullptr);
                                                const char* wn = defIdx ? WeaponNameFromDefIdx(defIdx) : nullptr;
                                                if (wn) {
                                                    strncpy(targets[i].weaponName, wn, sizeof(targets[i].weaponName)-1);
                                                } else {
                                                    // Fallback: designer string
                                                    uintptr_t ident = 0, namePtr = 0;
                                                    ReadProcessMemory(hGame, (LPCVOID)(wpnEnt + O::m_pEntity_id), &ident, sizeof(ident), nullptr);
                                                    if (ident) ReadProcessMemory(hGame, (LPCVOID)(ident + O::m_designerName), &namePtr, sizeof(namePtr), nullptr);
                                                    if (namePtr) {
                                                        char dnBuf[32] = {};
                                                        ReadProcessMemory(hGame, (LPCVOID)namePtr, dnBuf, sizeof(dnBuf)-1, nullptr);
                                                        const char* p = strstr(dnBuf, "weapon_");
                                                        strncpy(targets[i].weaponName, p ? p+7 : dnBuf, sizeof(targets[i].weaponName)-1);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } // for i
                    }
                }
            } // doCache

            // ---- High-speed hot pass: health + per-frame data for live entities ----
            int liveTargets = 0;
            for (int i = 1; i < 64; i++) {
                if (!targets[i].active || !targets[i].pawn) continue;
                if (targets[i].pawn == localPawn) continue;
                if (teamCheck && snap.localTeam && targets[i].team == snap.localTeam) continue;

                int health = 0;
                if (!ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_iHealth), &health, sizeof(health), nullptr) ||
                    health <= 0 || health > 100) continue;

                EntityData ed{};
                ed.valid       = true;
                ed.health      = health;
                ed.team        = targets[i].team;
                ed.entitySlot  = i;
                memcpy(ed.name,       targets[i].name,       sizeof(ed.name));
                memcpy(ed.weaponName, targets[i].weaponName, sizeof(ed.weaponName));

                // Armor
                ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_ArmorValue), &ed.armor, sizeof(ed.armor), nullptr);

                // Velocity (lead prediction — adapted from aimbot.cpp apply_prediction)
                ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_absVelocity), &ed.velocity, sizeof(ed.velocity), nullptr);

                // Shots fired (RCS gate)
                ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_iShotsFired), &ed.shotsFired, sizeof(ed.shotsFired), nullptr);

                // Eye angles (for eye ray draw)
                ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_angEyeAngles), &ed.eyeAngles, sizeof(ed.eyeAngles), nullptr);

                // Sound time — detect change, stamp wall clock (adapted from cs2_extras sound detection)
                {
                    float sndTime = 0.0f;
                    ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_emitSndTime), &sndTime, sizeof(sndTime), nullptr);
                    if (sndTime != targets[i].cachedSoundTime && sndTime > 0.0f) {
                        targets[i].cachedSoundTime = sndTime;
                        g_soundWallMs[i].store(GetTickCount64(), std::memory_order_relaxed);
                    }
                    ed.lastSoundTime = sndTime;
                }

                // Bones
                if (targets[i].boneArrayAddress) {
                    BoneJointData bj[23]{};
                    if (ReadProcessMemory(hGame, (LPCVOID)targets[i].boneArrayAddress, bj, sizeof(bj), nullptr)) {
                        ed.hasBones = true;
                        for (int b = 0; b < 23; b++) ed.bones[b] = bj[b].pos;
                        if (bj[0].pos.x || bj[0].pos.y || bj[0].pos.z) ed.origin  = bj[0].pos;
                        if (bj[7].pos.x || bj[7].pos.y || bj[7].pos.z) ed.headPos = bj[7].pos;
                    } else {
                        targets[i].boneArrayAddress = 0;
                    }
                }

                // Fallback origin/head
                if (!ed.hasBones || (!ed.origin.x && !ed.origin.y)) {
                    ReadProcessMemory(hGame, (LPCVOID)(targets[i].pawn + O::m_vOldOrigin), &ed.origin, sizeof(ed.origin), nullptr);
                    ed.headPos = {ed.origin.x, ed.origin.y, ed.origin.z + 65.0f};
                } else if (!ed.headPos.x && !ed.headPos.y) {
                    ed.headPos = {ed.origin.x, ed.origin.y, ed.origin.z + 65.0f};
                }

                if (snap.count < 64) snap.entities[snap.count++] = ed;
                liveTargets++;
            }
            telemetryTargets.store(liveTargets);
        }

        EnterCriticalSection(&g_snapLock);
        g_backSnapshot = snap;
        LeaveCriticalSection(&g_snapLock);

        auto tEnd = std::chrono::high_resolution_clock::now();
        telemetryScanMs.store(std::chrono::duration<float, std::milli>(tEnd - tStart).count());

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// ============ SLOW SCAN THREAD (bomb / loot / spectators — 500ms cadence) ============
// Architecture from CSGO_Public-main entity_worker.cpp + world_loot.cpp
void SlowScanThread() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (!hGame || !clientBase) continue;

        SlowSnapshot slow{};

        // ---- Planted C4 ----
        // Adapted from world_loot::refresh_bomb() — dwPlantedC4 indirect pointer
        {
            uintptr_t c4Ptr = 0;
            ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwPlantedC4), &c4Ptr, sizeof(c4Ptr), nullptr);
            if (c4Ptr) {
                uintptr_t c4Ent = 0;
                ReadProcessMemory(hGame, (LPCVOID)c4Ptr, &c4Ent, sizeof(c4Ent), nullptr);
                if (c4Ent) {
                    bool ticking = false;
                    ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bTicking), &ticking, 1, nullptr);
                    slow.bomb.ticking = ticking;
                    if (ticking) {
                        slow.bomb.valid = true;
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_nSite),      &slow.bomb.site,         sizeof(slow.bomb.site),         nullptr);
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flTimer),    &slow.bomb.timerLength,  sizeof(slow.bomb.timerLength),   nullptr);
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bDefusing),  &slow.bomb.defusing,     1,                               nullptr);
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_flDefuseLen),&slow.bomb.defuseLength, sizeof(slow.bomb.defuseLength),  nullptr);
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::c4_bDefused),   &slow.bomb.defused,      1,                               nullptr);
                        // Get world position via scene node
                        uintptr_t gsn = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(c4Ent + O::m_pGameSceneNode), &gsn, sizeof(gsn), nullptr);
                        if (gsn) ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_vecAbsOrigin), &slow.bomb.worldPos, sizeof(slow.bomb.worldPos), nullptr);
                    }
                }
            }

            // Merge plant wall-clock time from previous snapshot (preserve planted time across updates)
            EnterCriticalSection(&g_slowLock);
            if (slow.bomb.valid && slow.bomb.ticking) {
                if (g_slowSnap.bomb.wallTimeCaptured) {
                    slow.bomb.plantWallTime     = g_slowSnap.bomb.plantWallTime;
                    slow.bomb.wallTimeCaptured  = true;
                } else {
                    slow.bomb.plantWallTime    = std::chrono::steady_clock::now();
                    slow.bomb.wallTimeCaptured = true;
                }
            }
            LeaveCriticalSection(&g_slowLock);
        }

        // ---- World loot scan (dropped weapons) ----
        // Adapted from world_loot::refresh() — entity system walk with designer name filter
        if (worldLoot) {
            uintptr_t entityList = 0;
            ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList), &entityList, sizeof(entityList), nullptr);
            if (entityList) {
                // Walk slots 64–511 (weapons live outside player controller range)
                for (int i = 64; i < 512 && slow.itemCount < 64; i++) {
                    int chunkIdx = i >> 9;
                    int slotIdx  = i & 0x1FF;
                    uintptr_t chunkPtr = 0;
                    ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10 + 8 * chunkIdx), &chunkPtr, sizeof(chunkPtr), nullptr);
                    if (!chunkPtr) { if (i >= 128) break; continue; }
                    uintptr_t ent = 0;
                    ReadProcessMemory(hGame, (LPCVOID)(chunkPtr + 0x70 * slotIdx), &ent, sizeof(ent), nullptr);
                    if (!ent) ReadProcessMemory(hGame, (LPCVOID)(chunkPtr + 0x78 * slotIdx), &ent, sizeof(ent), nullptr);
                    if (!ent) continue;
                    // Read designer name
                    uintptr_t ident = 0;
                    ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_pEntity_id), &ident, sizeof(ident), nullptr);
                    if (!ident) continue;
                    uintptr_t namePtr = 0;
                    ReadProcessMemory(hGame, (LPCVOID)(ident + O::m_designerName), &namePtr, sizeof(namePtr), nullptr);
                    if (!namePtr) continue;
                    char dnBuf[48] = {};
                    ReadProcessMemory(hGame, (LPCVOID)namePtr, dnBuf, sizeof(dnBuf)-1, nullptr);
                    if (strncmp(dnBuf, "weapon_", 7) != 0) continue;
                    // Scene node → abs origin
                    uintptr_t gsn = 0;
                    ReadProcessMemory(hGame, (LPCVOID)(ent + O::m_pGameSceneNode), &gsn, sizeof(gsn), nullptr);
                    if (!gsn) continue;
                    Vec3 pos{};
                    ReadProcessMemory(hGame, (LPCVOID)(gsn + O::m_vecAbsOrigin), &pos, sizeof(pos), nullptr);
                    if (!pos.x && !pos.y && !pos.z) continue;
                    WorldItem& item = slow.items[slow.itemCount++];
                    strncpy(item.name, dnBuf + 7, sizeof(item.name)-1);
                    item.position = pos;
                    item.valid    = true;
                }
            }
        }

        // ---- Spectator list (adapted from cs2_extras::collect_spectators) ----
        // Find controllers whose observed pawn == local player pawn
        if (showSpectators) {
            uintptr_t localPawn = 0;
            ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwLocalPlayerPawn), &localPawn, sizeof(localPawn), nullptr);
            uintptr_t entityList = 0;
            ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwEntityList), &entityList, sizeof(entityList), nullptr);

            if (localPawn && entityList) {
                uintptr_t listEntry = 0;
                ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10), &listEntry, sizeof(listEntry), nullptr);
                if (listEntry) {
                    for (int i = 1; i < 64 && slow.spectatorCount < 8; i++) {
                        uintptr_t ctrl = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(listEntry + i * 0x70), &ctrl, sizeof(ctrl), nullptr);
                        if (!ctrl) continue;
                        uint32_t pawnH = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(ctrl + O::m_hPlayerPawn), &pawnH, sizeof(pawnH), nullptr);
                        if (!pawnH || pawnH == 0xFFFFFFFF) continue;
                        // Resolve pawn
                        uintptr_t ple = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10 + 8 * ((pawnH & 0x7FFF) >> 9)), &ple, sizeof(ple), nullptr);
                        if (!ple) continue;
                        uintptr_t pawn = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(ple + 0x70 * (pawnH & 0x1FF)), &pawn, sizeof(pawn), nullptr);
                        if (!pawn || pawn == localPawn) continue;
                        // Observer services
                        uintptr_t obsSvc = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(pawn + O::m_pObserverSvc), &obsSvc, sizeof(obsSvc), nullptr);
                        if (!obsSvc) continue;
                        uint32_t obsTgtH = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(obsSvc + O::m_hObsTgt), &obsTgtH, sizeof(obsTgtH), nullptr);
                        if (!obsTgtH || obsTgtH == 0xFFFFFFFF) continue;
                        // Resolve observed pawn
                        uintptr_t tple = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(entityList + 0x10 + 8 * ((obsTgtH & 0x7FFF) >> 9)), &tple, sizeof(tple), nullptr);
                        if (!tple) continue;
                        uintptr_t tPawn = 0;
                        ReadProcessMemory(hGame, (LPCVOID)(tple + 0x70 * (obsTgtH & 0x1FF)), &tPawn, sizeof(tPawn), nullptr);
                        if (tPawn != localPawn) continue;
                        // This player watches local — get name
                        char nb[64] = {};
                        ReadProcessMemory(hGame, (LPCVOID)(ctrl + O::m_iszPlayerName), nb, sizeof(nb)-1, nullptr);
                        if (nb[0]) strncpy(slow.spectators[slow.spectatorCount++].name, nb, 63);
                    }
                }
            }
        }

        EnterCriticalSection(&g_slowLock);
        g_slowSnap = slow;
        LeaveCriticalSection(&g_slowLock);
    }
}

// ============ AIMBOT TICK (adapted from cAimbot — CSGO_Public-main aimbot.cpp) ============
// Uses SendInput (same-machine). For hardware mouse use KMBox/Makcu instead.
static void AimbotTick(const FrameSnapshot& snap) {
    if (!g_masterEnabled || !aimbotEnabled) return;

    // Aim key check
    const int aimVkeys[] = { VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT };
    int vk = (aimbotKey >= 0 && aimbotKey < 4) ? aimVkeys[aimbotKey] : VK_LMENU;
    if (!(GetAsyncKeyState(vk) & 0x8000)) return;

    float cx = screenWidth  / 2.0f;
    float cy = screenHeight / 2.0f;
    // FOV → pixel radius (approximate vertical-FOV-to-pixel conversion)
    float fovPx   = aimbotFovDeg * ((float)screenHeight / 90.0f);
    int   aimBone = (aimbotBone == 0) ? 7 : (aimbotBone == 1) ? 6 : 4;

    float bestDist  = fovPx;
    Vec2  bestScr   = {cx, cy};
    Vec3  bestWorld = {};
    Vec3  bestVel   = {};
    Vec3  bestOrigin= {};
    bool  found     = false;

    for (int e = 0; e < snap.count; e++) {
        const auto& ent = snap.entities[e];
        if (!ent.valid || ent.health <= 0) continue;
        if (teamCheck && snap.localTeam && ent.team == snap.localTeam) continue;
        if (!ent.hasBones) continue;

        Vec3 aimWorld = ent.bones[aimBone];
        if (!aimWorld.x && !aimWorld.y && !aimWorld.z) continue;

        Vec2 aimScr;
        if (!WorldToScreen(aimWorld, snap.viewMatrix, aimScr)) continue;

        float dx = aimScr.x - cx, dy = aimScr.y - cy;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < bestDist) {
            bestDist   = dist;
            bestScr    = aimScr;
            bestWorld  = aimWorld;
            bestVel    = ent.velocity;
            bestOrigin = ent.origin;
            found      = true;
        }
    }
    if (!found) return;

    // Velocity lead prediction (adapted from apply_prediction in aimbot.cpp)
    if (aimbotLead) {
        Vec3 toTarget = {bestWorld.x - snap.localOrigin.x,
                         bestWorld.y - snap.localOrigin.y,
                         bestWorld.z - snap.localOrigin.z};
        float dist3d = std::sqrt(toTarget.x*toTarget.x + toTarget.y*toTarget.y + toTarget.z*toTarget.z);
        // 75000 units/s ≈ rifle muzzle velocity in CS2 world units
        float tof = dist3d / 75000.0f;
        Vec3 ledWorld = bestWorld + bestVel * tof;
        Vec2 ledScr;
        if (WorldToScreen(ledWorld, snap.viewMatrix, ledScr)) bestScr = ledScr;
    }

    float movX = (bestScr.x - cx) / (aimbotSmooth > 0.1f ? aimbotSmooth : 1.0f);
    float movY = (bestScr.y - cy) / (aimbotSmooth > 0.1f ? aimbotSmooth : 1.0f);
    movX = std::clamp(movX, -150.0f, 150.0f);
    movY = std::clamp(movY, -150.0f, 150.0f);

    INPUT inp = {};
    inp.type = INPUT_MOUSE;
    inp.mi.dwFlags = MOUSEEVENTF_MOVE;
    inp.mi.dx = (LONG)roundf(movX);
    inp.mi.dy = (LONG)roundf(movY);
    if (inp.mi.dx || inp.mi.dy) SendInput(1, &inp, sizeof(INPUT));
}

// ============ TRIGGERBOT TICK (adapted from TriggerBot.cpp method-3 proximity) ============
static auto s_lastTrigTime = std::chrono::steady_clock::now();
static void TriggerbotTick(const FrameSnapshot& snap) {
    if (!g_masterEnabled || !triggerbotEnabled) return;

    if (triggerMode == 0) {
        // Key-hold mode: Caps Lock as default trigger key
        if (!(GetAsyncKeyState(VK_CAPITAL) & 0x8000)) return;
    }
    // Prevent firing while left mouse is already held
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) return;

    float cx = screenWidth  / 2.0f;
    float cy = screenHeight / 2.0f;
    constexpr float kTriggerRadius = 18.0f; // pixels from crosshair center

    bool onTarget = false;
    for (int e = 0; e < snap.count && !onTarget; e++) {
        const auto& ent = snap.entities[e];
        if (!ent.valid || ent.health <= 0) continue;
        if (teamCheck && snap.localTeam && ent.team == snap.localTeam) continue;
        if (!ent.hasBones) continue;

        // Check head (7) and chest/spine (4) bones
        for (int b : {7, 4}) {
            Vec3 bw = ent.bones[b];
            if (!bw.x && !bw.y && !bw.z) continue;
            Vec2 bs;
            if (!WorldToScreen(bw, snap.viewMatrix, bs)) continue;
            float d = std::sqrt((bs.x-cx)*(bs.x-cx) + (bs.y-cy)*(bs.y-cy));
            if (d < kTriggerRadius) { onTarget = true; break; }
        }
    }
    if (!onTarget) return;

    auto now = std::chrono::steady_clock::now();
    int elapsed = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastTrigTime).count();
    if (elapsed < triggerDelay + 30) return;
    s_lastTrigTime = now;

    INPUT inp[2] = {};
    inp[0].type = INPUT_MOUSE; inp[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inp[1].type = INPUT_MOUSE; inp[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inp, sizeof(INPUT));
}

// ============ DIRECTX 11 SETUP ============
bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd; ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate = {60, 1};
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd; sd.SampleDesc.Count = 1; sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    UINT flags = 0;
    D3D_FEATURE_LEVEL fl;
    const D3D_FEATURE_LEVEL fla[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, fla, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &fl, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, flags, fla, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &fl, &g_pd3dDeviceContext);
    if (res != S_OK) return false;
    ID3D11Texture2D* pBB; g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBB));
    g_pd3dDevice->CreateRenderTargetView(pBB, NULL, &g_mainRenderTargetView); pBB->Release();
    D3D11_BLEND_DESC bd = {};
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend  = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
    float bf[4] = {}; g_pd3dDeviceContext->OMSetBlendState(g_pBlendState, bf, 0xffffffff);
    return true;
}

void CleanupDeviceD3D() {
    if (g_pBlendState)          { g_pBlendState->Release();          g_pBlendState = NULL; }
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release();  g_mainRenderTargetView = NULL; }
    if (g_pSwapChain)           { g_pSwapChain->Release();           g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext)    { g_pd3dDeviceContext->Release();     g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice)           { g_pd3dDevice->Release();           g_pd3dDevice = NULL; }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    switch (msg) {
        case WM_SIZE:
            if (g_pd3dDevice && wParam != SIZE_MINIMIZED) {
                if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
                g_pSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                ID3D11Texture2D* pBB; g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBB));
                g_pd3dDevice->CreateRenderTargetView(pBB, NULL, &g_mainRenderTargetView); pBB->Release();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
            break;
        case WM_NCHITTEST: {
            if (!menuOpen) return HTTRANSPARENT;
            ImGuiContext* g = ImGui::GetCurrentContext();
            if (g) {
                POINT pt = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
                ScreenToClient(hWnd, &pt);
                ImVec2 mp((float)pt.x, (float)pt.y);
                if (g->MovingWindow || ImGui::GetIO().MouseDown[0] || ImGui::GetIO().MouseDown[1]) return HTCLIENT;
                ImGuiWindow* win = ImGui::FindWindowByName("General");
                if (win && win->Active && !win->Hidden) {
                    ImRect bb(win->Pos, ImVec2(win->Pos.x+win->Size.x, win->Pos.y+win->Size.y));
                    bb.Expand(6.0f);
                    if (bb.Contains(mp)) return HTCLIENT;
                }
                if (g->OpenPopupStack.Size > 0) return HTCLIENT;
            }
            return HTTRANSPARENT;
        }
        case WM_DESTROY: ::PostQuitMessage(0); return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// ============ WINMAIN ============
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    // Timer resolution guard — raises scheduler quantum to 1 ms for the process lifetime.
    // Adapted directly from CSGO_Public-main Main.cpp timer_resolution_guard struct.
    TimerGuard _timer;

    InitializeCriticalSection(&g_snapLock);
    InitializeCriticalSection(&g_slowLock);

    int posX = 0, posY = 0;
    HWND hwndCS2 = FindWindowA("SDL_app", "Counter-Strike 2");
    if (!hwndCS2) hwndCS2 = FindWindowA(NULL, "Counter-Strike 2");
    if (hwndCS2) {
        GetWindowThreadProcessId(hwndCS2, &pid);
        RECT rect; GetClientRect(hwndCS2, &rect);
        if (rect.right - rect.left > 0) screenWidth  = rect.right - rect.left;
        if (rect.bottom - rect.top  > 0) screenHeight = rect.bottom - rect.top;
        POINT pt = {0,0}; ClientToScreen(hwndCS2, &pt);
        posX = pt.x; posY = pt.y;
    }

    WNDCLASSEXW wc = {sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, L"CS2_Ostin_ESP", NULL};
    ::RegisterClassExW(&wc);
    hwndOverlay = ::CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName, L"CS2 Ostin ESP Overlay",
        WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, wc.hInstance, NULL);
    SetLayeredWindowAttributes(hwndOverlay, 0, 255, LWA_ALPHA);
    MARGINS margins = {-1,-1,-1,-1};
    DwmExtendFrameIntoClientArea(hwndOverlay, &margins);

    if (!CreateDeviceD3D(hwndOverlay)) { CleanupDeviceD3D(); ::UnregisterClassW(wc.lpszClassName, wc.hInstance); return 1; }
    ::ShowWindow(hwndOverlay, SW_SHOWNOACTIVATE);
    ::UpdateWindow(hwndOverlay);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    ImGui_ImplWin32_Init(hwndOverlay);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Fonts
    Inter_S   = io.Fonts->AddFontFromMemoryTTF(&Inter_SemmiBold, sizeof(Inter_SemmiBold), 17.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_S_1 = io.Fonts->AddFontFromMemoryTTF(&Inter_SemmiBold, sizeof(Inter_SemmiBold), 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_S_2 = io.Fonts->AddFontFromMemoryTTF(&Inter_SemmiBold, sizeof(Inter_SemmiBold), 23.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_S_3 = io.Fonts->AddFontFromMemoryTTF(&Inter_SemmiBold, sizeof(Inter_SemmiBold), 15.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_B   = io.Fonts->AddFontFromMemoryTTF(&Inter_Bold,       sizeof(Inter_Bold),      34.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Icon      = io.Fonts->AddFontFromMemoryTTF(&Icon_Pack,        sizeof(Icon_Pack),        26.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Icon_Arrow= io.Fonts->AddFontFromMemoryTTF(&Arrow,            sizeof(Arrow),             6.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Images
    if (!image::settings_checkbox) image::settings_checkbox = CreateSRVFromMemoryDX11(g_pd3dDevice, settings_checkbox, sizeof(settings_checkbox));
    if (!image::user)  image::user  = CreateSRVFromMemoryDX11(g_pd3dDevice, user_icon,  sizeof(user_icon));
    if (!image::fps)   image::fps   = CreateSRVFromMemoryDX11(g_pd3dDevice, fps_icon,   sizeof(fps_icon));
    if (!image::ms)    image::ms    = CreateSRVFromMemoryDX11(g_pd3dDevice, ms_icon,    sizeof(ms_icon));
    if (!image::logo)  image::logo  = CreateSRVFromMemoryDX11(g_pd3dDevice, Logo_icon,  sizeof(Logo_icon));

    // Start threads
    std::thread memThread(MemoryWorkerThread);
    std::thread slowThread(SlowScanThread);

    bool lastInsertDown = false;
    bool lastHomeDown   = false;
    auto lastFrameTime  = std::chrono::high_resolution_clock::now();
    int  frameCounter   = 0;
    float fpsTimer      = 0.0f;

    MSG msg; ZeroMemory(&msg, sizeof(msg));
    while (running && msg.message != WM_QUIT) {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&msg); ::DispatchMessage(&msg); continue;
        }

        // Dynamic CS2 window bounds sync (rate-limited)
        static int winCheckTicks = 0; static HWND hwndCS2Loop = NULL;
        if (++winCheckTicks >= 30 || !hwndCS2Loop || !IsWindow(hwndCS2Loop)) {
            winCheckTicks = 0;
            hwndCS2Loop = FindWindowA("SDL_app", "Counter-Strike 2");
            if (!hwndCS2Loop) hwndCS2Loop = FindWindowA(NULL, "Counter-Strike 2");
            if (hwndCS2Loop) {
                RECT r; if (GetClientRect(hwndCS2Loop, &r)) {
                    int w = r.right - r.left, h = r.bottom - r.top;
                    POINT pt = {0,0}; ClientToScreen(hwndCS2Loop, &pt);
                    static int lx=-9999, ly=-9999;
                    if (w > 0 && h > 0 && (w!=screenWidth||h!=screenHeight||pt.x!=lx||pt.y!=ly)) {
                        screenWidth = w; screenHeight = h; lx = pt.x; ly = pt.y;
                        SetWindowPos(hwndOverlay, HWND_TOPMOST, pt.x, pt.y, w, h, SWP_NOACTIVATE|SWP_SHOWWINDOW);
                    }
                }
            }
        }

        // INSERT — menu toggle
        bool insertDown = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
        if (insertDown && !lastInsertDown) menuOpen = !menuOpen;
        lastInsertDown = insertDown;

        // HOME — panic master toggle (from CSGO_Public-main Main.cpp panic_key logic)
        bool homeDown = (GetAsyncKeyState(VK_HOME) & 0x8000) != 0;
        if (homeDown && !lastHomeDown) g_masterEnabled = !g_masterEnabled;
        lastHomeDown = homeDown;

        // Selective click-through
        static bool isCT = true; bool shouldBeCT = true;
        if (menuOpen) {
            POINT cur; GetCursorPos(&cur); ScreenToClient(hwndOverlay, &cur);
            ImGuiWindow* win = ImGui::FindWindowByName("General");
            if (win && win->Active && !win->Hidden) {
                ImRect bb(win->Pos, ImVec2(win->Pos.x+win->Size.x, win->Pos.y+win->Size.y)); bb.Expand(6.f);
                if (bb.Contains(ImVec2((float)cur.x,(float)cur.y))) shouldBeCT = false;
            }
            ImGuiContext* g = ImGui::GetCurrentContext();
            if (g && (g->MovingWindow || io.MouseDown[0] || io.MouseDown[1] || g->OpenPopupStack.Size > 0)) shouldBeCT = false;
        }
        if (isCT != shouldBeCT) {
            isCT = shouldBeCT;
            LONG_PTR ex = GetWindowLongPtrA(hwndOverlay, GWL_EXSTYLE);
            SetWindowLongPtrA(hwndOverlay, GWL_EXSTYLE, shouldBeCT ? (ex|WS_EX_TRANSPARENT) : (ex&~WS_EX_TRANSPARENT));
        }

        // FPS
        auto cur = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(cur - lastFrameTime).count(); lastFrameTime = cur;
        if ((fpsTimer += dt) >= 1.0f) { telemetryFps.store(++frameCounter); frameCounter = 0; fpsTimer = 0.f; }
        else ++frameCounter;

        // Copy snapshot
        FrameSnapshot snap;
        EnterCriticalSection(&g_snapLock);
        snap = g_backSnapshot;
        LeaveCriticalSection(&g_snapLock);

        SlowSnapshot slowLocal;
        EnterCriticalSection(&g_slowLock);
        slowLocal = g_slowSnap;
        LeaveCriticalSection(&g_slowLock);

        // Real-time view matrix sync (eliminates camera drag)
        if (hGame && clientBase)
            ReadProcessMemory(hGame, (LPCVOID)(clientBase + O::dwViewMatrix), &snap.viewMatrix, sizeof(snap.viewMatrix), nullptr);

        // Aimbot + triggerbot tick (after fresh snapshot)
        AimbotTick(snap);
        TriggerbotTick(snap);

        // ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        CustomStyleColor();

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        // ---- Master disabled overlay ----
        if (!g_masterEnabled) {
            const char* txt = "[PAUSED — HOME to resume]";
            ImVec2 tsz = ImGui::CalcTextSize(txt);
            drawList->AddText(ImVec2(screenWidth/2.0f - tsz.x/2.0f + 1, 11.f), IM_COL32(0,0,0,220), txt);
            drawList->AddText(ImVec2(screenWidth/2.0f - tsz.x/2.0f,     10.f), IM_COL32(255,80,80,255), txt);
        }

        // ---- Status watermark ----
        if (!menuOpen) {
            char statusBuf[160];
            sprintf(statusBuf, "TRUST CODE  |  %d TGT  |  %d FPS  |  %s  [INSERT: MENU | HOME: PANIC]",
                    telemetryTargets.load(), telemetryFps.load(),
                    g_masterEnabled ? "ON" : "OFF");
            drawList->AddText(ImVec2(16, 16), IM_COL32(0,0,0,220), statusBuf);
            drawList->AddText(ImVec2(15, 15), IM_COL32(0, 230, 255, 200), statusBuf);
        }

        // ==== ESP RENDER PASS ====
        if (espEnabled && g_masterEnabled && snap.count > 0) {
            ImU32 boxColor = (colorPalette==1) ? IM_COL32(0,230,255,255) :
                             (colorPalette==2) ? IM_COL32(0,255,128,255) :
                             (colorPalette==3) ? IM_COL32(255,40,80,255) :
                                                 IM_COL32(140,90,255,255);
            // Hoisted constants — computed once per frame, not per-entity
            const ImU32 kBoxOutl = IM_COL32(0, 0, 0, 220);
            const ImU32 kBarOutl = IM_COL32(0, 0, 0, 220);

            for (int e = 0; e < snap.count; e++) {
                const auto& ent = snap.entities[e];
                if (!ent.valid) continue;

                Vec2 headScr, originScr;
                if (!WorldToScreen(ent.headPos, snap.viewMatrix, headScr) ||
                    !WorldToScreen(ent.origin,  snap.viewMatrix, originScr)) continue;

                float boxH = (originScr.y - headScr.y) * 1.09f;
                if (boxH < 5.0f) boxH = 5.0f;
                float boxW     = boxH * 0.60f;
                float boxTop   = headScr.y - (boxH * 0.08f);
                float boxLeft  = originScr.x - (boxW / 2.0f);
                float boxRight = boxLeft + boxW;
                float boxBtm   = boxTop + boxH;

                // Compute distance once — reused by skeleton alpha AND name/distance label.
                // DistTo calls sqrtf internally; doing it twice per entity doubles the cost.
                const float distU  = snap.localOrigin.DistTo(ent.origin);
                const float distM  = distU * 0.0254f; // world units → metres

                // 1. Box ESP
                if (drawBoxes) {
                    if (boxStyle == 0) {
                        // Corner brackets — kBoxOutl hoisted out of entity loop
                        float cL = boxW * 0.25f;
                        drawList->AddLine({boxLeft-1,boxTop},   {boxLeft+cL+1,boxTop},   kBoxOutl, 3.5f);
                        drawList->AddLine({boxLeft,boxTop-1},   {boxLeft,boxTop+cL+1},   kBoxOutl, 3.5f);
                        drawList->AddLine({boxLeft,boxTop},     {boxLeft+cL,boxTop},     boxColor, 1.8f);
                        drawList->AddLine({boxLeft,boxTop},     {boxLeft,boxTop+cL},     boxColor, 1.8f);
                        drawList->AddLine({boxRight-cL-1,boxTop},{boxRight+1,boxTop},    kBoxOutl, 3.5f);
                        drawList->AddLine({boxRight,boxTop-1},  {boxRight,boxTop+cL+1},  kBoxOutl, 3.5f);
                        drawList->AddLine({boxRight-cL,boxTop}, {boxRight,boxTop},       boxColor, 1.8f);
                        drawList->AddLine({boxRight,boxTop},    {boxRight,boxTop+cL},    boxColor, 1.8f);
                        drawList->AddLine({boxLeft-1,boxBtm},   {boxLeft+cL+1,boxBtm},  kBoxOutl, 3.5f);
                        drawList->AddLine({boxLeft,boxBtm-cL-1},{boxLeft,boxBtm+1},      kBoxOutl, 3.5f);
                        drawList->AddLine({boxLeft,boxBtm},     {boxLeft+cL,boxBtm},    boxColor, 1.8f);
                        drawList->AddLine({boxLeft,boxBtm-cL},  {boxLeft,boxBtm},       boxColor, 1.8f);
                        drawList->AddLine({boxRight-cL-1,boxBtm},{boxRight+1,boxBtm},   kBoxOutl, 3.5f);
                        drawList->AddLine({boxRight,boxBtm-cL-1},{boxRight,boxBtm+1},   kBoxOutl, 3.5f);
                        drawList->AddLine({boxRight-cL,boxBtm}, {boxRight,boxBtm},      boxColor, 1.8f);
                        drawList->AddLine({boxRight,boxBtm-cL}, {boxRight,boxBtm},      boxColor, 1.8f);
                    } else {
                        drawList->AddRect({boxLeft-1,boxTop-1},{boxRight+1,boxBtm+1}, kBoxOutl, 0, 0, 3.5f);
                        drawList->AddRect({boxLeft,boxTop},{boxRight,boxBtm}, boxColor, 0, 0, 1.8f);
                    }
                }

                // 2. Health bar (left side, vertical)
                if (drawHealth) {
                    float bW = 3.5f, bL = boxLeft - 7.0f;
                    float pct = std::clamp(ent.health / 100.0f, 0.0f, 1.0f);
                    float hH  = boxH * pct;
                    drawList->AddRectFilled({bL-1,boxTop-1},{bL+bW+1,boxBtm+1}, kBarOutl);
                    ImU32 hpCol = (pct>0.5f) ? IM_COL32(0,255,120,255) : (pct>0.25f ? IM_COL32(255,200,0,255) : IM_COL32(255,40,40,255));
                    drawList->AddRectFilled({bL,boxBtm-hH},{bL+bW,boxBtm}, hpCol);
                    // Health value — clamped to screen bottom so it doesn't clip
                    char hpTxt[8]; snprintf(hpTxt, sizeof(hpTxt), "%d", ent.health);
                    if (boxBtm + 14.f < (float)screenHeight)
                        drawList->AddText({bL-2.f, boxBtm+2.f}, IM_COL32(200,200,200,220), hpTxt);
                }

                // 3. Armor bar (right side, vertical)
                if (drawArmor && ent.armor > 0) {
                    float bW = 3.5f, bR = boxRight + 4.0f;
                    float pct = std::clamp(ent.armor / 100.0f, 0.0f, 1.0f);
                    float aH  = boxH * pct;
                    drawList->AddRectFilled({bR-1,boxTop-1},{bR+bW+1,boxBtm+1}, kBarOutl);
                    drawList->AddRectFilled({bR,boxBtm-aH},{bR+bW,boxBtm}, IM_COL32(80,180,255,230));
                }

                // 4. Head dot
                if (drawHeadDot) {
                    float r = std::max(2.0f, boxH * 0.045f);
                    drawList->AddCircleFilled({headScr.x,headScr.y}, r+1.0f, IM_COL32(0,0,0,220));
                    drawList->AddCircleFilled({headScr.x,headScr.y}, r,      IM_COL32(255,255,255,255));
                }

                // 5. Skeleton ESP — mode selectable (0 = Optimized Stick Figure, 1 = Full Detailed Skeleton)
                if (drawSkeletons && ent.hasBones) {
                    static constexpr struct { uint8_t a, b; } kLinksOpt[] = {
                        {7,6},{6,1},
                        {6,11},
                        {6,15},
                        {1,19},
                        {1,22}
                    };
                    static constexpr uint8_t kUniqueOpt[] = {1,6,7,11,15,19,22};

                    static constexpr struct { uint8_t a, b; } kLinksFull[] = {
                        {7,6},{6,4},{4,1},
                        {6,9},{9,10},{10,11},
                        {6,13},{13,14},{14,15},
                        {1,17},{17,18},{18,19},
                        {1,20},{20,21},{21,22}
                    };
                    static constexpr uint8_t kUniqueFull[] = {1,4,6,7,9,10,11,13,14,15,17,18,19,20,21,22};

                    const uint8_t* uniqueBones = (skeletonStyle == 0) ? kUniqueOpt : kUniqueFull;
                    size_t uniqueCount = (skeletonStyle == 0) ? sizeof(kUniqueOpt) : sizeof(kUniqueFull);

                    // Pre-project unique bones into screen-space cache
                    Vec2 scr[23];
                    bool ok[23] = {};
                    for (size_t i = 0; i < uniqueCount; i++) {
                        uint8_t bi = uniqueBones[i];
                        const Vec3& bw = ent.bones[bi];
                        if (bw.x || bw.y || bw.z)
                            ok[bi] = WorldToScreen(bw, snap.viewMatrix, scr[bi]);
                    }

                    // Distance-based alpha — reuses distU computed once per entity above
                    uint8_t skelA = (uint8_t)std::clamp(220.0f - distU * 0.04f, 100.0f, 220.0f);

                    ImU32 shadowCol = IM_COL32(0, 0, 0, (uint8_t)(skelA * 0.7f));
                    ImVec4 bc = ImGui::ColorConvertU32ToFloat4(boxColor);
                    ImU32 skelCol = IM_COL32(
                        (uint8_t)(bc.x * 153 + 102),
                        (uint8_t)(bc.y * 153 + 102),
                        (uint8_t)(bc.z * 153 + 102),
                        skelA);

                    auto renderLinks = [&](const auto* links, size_t count) {
                        for (size_t i = 0; i < count; i++) {
                            const auto& lnk = links[i];
                            if (!ok[lnk.a] || !ok[lnk.b]) continue;
                            drawList->AddLine(
                                {scr[lnk.a].x+1.f, scr[lnk.a].y+1.f},
                                {scr[lnk.b].x+1.f, scr[lnk.b].y+1.f},
                                shadowCol, 2.5f);
                            drawList->AddLine(
                                {scr[lnk.a].x, scr[lnk.a].y},
                                {scr[lnk.b].x, scr[lnk.b].y},
                                skelCol, 1.5f);
                        }
                    };

                    if (skeletonStyle == 0) {
                        renderLinks(kLinksOpt, sizeof(kLinksOpt) / sizeof(kLinksOpt[0]));
                    } else {
                        renderLinks(kLinksFull, sizeof(kLinksFull) / sizeof(kLinksFull[0]));
                    }
                }

                // 6. Name + distance — uses distM already computed from distU above (no extra sqrt)
                if (drawNames || drawDistance) {
                    char lblBuf[128] = {};
                    if (drawNames && drawDistance) snprintf(lblBuf, sizeof(lblBuf), "%s (%.0fm)", ent.name, distM);
                    else if (drawNames)            snprintf(lblBuf, sizeof(lblBuf), "%s", ent.name);
                    else                           snprintf(lblBuf, sizeof(lblBuf), "%.0fm", distM);
                    ImVec2 tsz = ImGui::CalcTextSize(lblBuf);
                    float tx = originScr.x - tsz.x/2.0f, ty = boxTop - tsz.y - 3.0f;
                    drawList->AddText({tx+1,ty+1}, IM_COL32(0,0,0,255), lblBuf);
                    drawList->AddText({tx,ty},     IM_COL32(255,255,255,255), lblBuf);
                }

                // 7. Weapon name (below box — adapted from base/Cheats.cpp weapon ESP)
                if (drawWeapon && ent.weaponName[0]) {
                    ImVec2 tsz = ImGui::CalcTextSize(ent.weaponName);
                    float tx = originScr.x - tsz.x/2.0f, ty = boxBtm + 2.0f;
                    drawList->AddText({tx+1,ty+1}, IM_COL32(0,0,0,200), ent.weaponName);
                    drawList->AddText({tx,ty},     IM_COL32(255,220,80,230), ent.weaponName);
                }

                // 8. Snap lines
                if (drawSnaplines)
                    drawList->AddLine({screenWidth/2.0f, (float)screenHeight}, {originScr.x, originScr.y}, IM_COL32(255,255,255,120), 1.2f);

                // 9. Eye ray — reuses headScr (already valid at this scope) + ent.headPos
                // Removed bones[7] re-read + redundant validity check: headScr being valid
                // guarantees ent.headPos is non-zero (set from bones[7] in the worker).
                if (drawEyeRay) {
                    float pitch = ent.eyeAngles.x * (3.14159f / 180.0f);
                    float yaw   = ent.eyeAngles.y * (3.14159f / 180.0f);
                    float cp = std::cos(pitch), sp = std::sin(pitch);
                    float cy = std::cos(yaw),   sy = std::sin(yaw);
                    constexpr float kRayLen = 120.0f;
                    Vec3 endW = {
                        ent.headPos.x + cp * cy  * kRayLen,
                        ent.headPos.y + cp * sy  * kRayLen,
                        ent.headPos.z - sp       * kRayLen
                    };
                    Vec2 endScr;
                    if (WorldToScreen(endW, snap.viewMatrix, endScr))
                        drawList->AddLine({headScr.x,headScr.y},{endScr.x,endScr.y}, IM_COL32(255,60,60,200), 1.3f);
                }
            } // for each entity
        } // espEnabled

        // ==== SOUND ESP RINGS (adapted from sound_esp.cpp draw_audio_rings) ====
        if (soundEsp && g_masterEnabled) {
            // Precomputed trig table — static one-time init, eliminates 36 cos/sin calls
            // per active sound entity per frame (18 steps × 2 functions).
            static float kRingCos[18], kRingSin[18];
            static bool  kRingInit = false;
            if (!kRingInit) {
                constexpr float kStep = 2.0f * 3.14159f / 18.0f;
                for (int i = 0; i < 18; i++) { kRingCos[i] = std::cos(i*kStep); kRingSin[i] = std::sin(i*kStep); }
                kRingInit = true;
            }

            uint64_t nowMs = GetTickCount64();
            for (int e = 0; e < snap.count; e++) {
                const auto& ent = snap.entities[e];
                if (!ent.valid) continue;
                if (teamCheck && snap.localTeam && ent.team == snap.localTeam) continue;
                if (ent.entitySlot < 0 || ent.entitySlot >= 64) continue;

                uint64_t lastMs  = g_soundWallMs[ent.entitySlot].load(std::memory_order_relaxed);
                if (!lastMs) continue;
                int64_t  elapsed = (int64_t)(nowMs - lastMs);
                if (elapsed < 0 || elapsed > 2500) continue;

                float alpha   = 1.0f - (float)elapsed / 2500.0f;
                ImU32 ringCol = IM_COL32(255, 100, 0, (int)(alpha * 200));
                float radius  = 55.0f + (float)elapsed * 0.025f;

                // Draw ring using precomputed table — close the loop by wrapping index 0
                Vec2 prev; bool prevOk = false;
                for (int s = 0; s <= 18; s++) {
                    int idx = s % 18;
                    Vec3 wp = { ent.origin.x + kRingCos[idx]*radius,
                                ent.origin.y + kRingSin[idx]*radius,
                                ent.origin.z };
                    Vec2 sp; bool valid = WorldToScreen(wp, snap.viewMatrix, sp);
                    if (valid && prevOk)
                        drawList->AddLine({prev.x,prev.y},{sp.x,sp.y}, ringCol, 1.6f);
                    prev = sp; prevOk = valid;
                }
                // HUD indicator dot above head
                Vec2 hScr;
                if (WorldToScreen(ent.headPos, snap.viewMatrix, hScr))
                    drawList->AddCircleFilled({hScr.x, hScr.y - 12.f}, 4.f, IM_COL32(255,120,0,(int)(alpha*255)));
            }
        }

        // ==== FOV CIRCLE (aimbot range indicator) ====
        // 40 segments — visually identical to 64 at typical FOV circle radii (< 300px)
        if (aimbotEnabled && aimbotFovCircle && g_masterEnabled) {
            float fovPx = aimbotFovDeg * ((float)screenHeight / 90.0f);
            float cx = screenWidth/2.0f, cy = screenHeight/2.0f;
            drawList->AddCircle({cx,cy}, fovPx, IM_COL32(255,255,255,90), 40, 1.2f);
        }

        // ==== BOMB ESP (timer + site + defuse bar) ====
        if (bombEsp && g_masterEnabled && slowLocal.bomb.valid && slowLocal.bomb.ticking) {
            float elapsed = slowLocal.bomb.wallTimeCaptured ?
                std::chrono::duration<float>(std::chrono::steady_clock::now() - slowLocal.bomb.plantWallTime).count() : 0.f;
            float remaining = std::max(0.0f, slowLocal.bomb.timerLength - elapsed);
            float pct       = slowLocal.bomb.timerLength > 0 ? (remaining / slowLocal.bomb.timerLength) : 0.f;

            const char* siteStr = (slowLocal.bomb.site==0) ? "A" : (slowLocal.bomb.site==1) ? "B" : "?";
            char bombBuf[80];
            if (slowLocal.bomb.defused)
                snprintf(bombBuf, sizeof(bombBuf), "[DEFUSED]");
            else if (slowLocal.bomb.defusing)
                snprintf(bombBuf, sizeof(bombBuf), "[%s] %.1fs | DEFUSING...", siteStr, remaining);
            else
                snprintf(bombBuf, sizeof(bombBuf), "[BOMB %s] %.1fs", siteStr, remaining);

            ImU32 bombCol = (pct > 0.5f) ? IM_COL32(0,220,80,255) :
                            (pct > 0.25f) ? IM_COL32(255,180,0,255) : IM_COL32(255,40,40,255);

            ImVec2 tsz = ImGui::CalcTextSize(bombBuf);
            float bx = screenWidth/2.0f - tsz.x/2.0f;
            float by = (float)screenHeight - 72.0f;
            drawList->AddRectFilled({bx-10,by-6},{bx+tsz.x+10,by+tsz.y+20}, IM_COL32(0,0,0,180), 4.0f);
            drawList->AddText({bx,by}, bombCol, bombBuf);

            // Timer bar
            float barW = 220.0f, barH = 5.0f;
            float barX = screenWidth/2.0f - barW/2.0f, barY = by + tsz.y + 8.0f;
            drawList->AddRectFilled({barX,barY},{barX+barW,barY+barH}, IM_COL32(30,30,30,200), 2.f);
            drawList->AddRectFilled({barX,barY},{barX+barW*pct,barY+barH}, bombCol, 2.f);

            // World-space icon if visible
            Vec2 c4Scr;
            if (WorldToScreen(slowLocal.bomb.worldPos, snap.viewMatrix, c4Scr)) {
                drawList->AddCircle({c4Scr.x,c4Scr.y}, 8.f, IM_COL32(255,80,80,255), 12, 2.f);
                drawList->AddText({c4Scr.x+10.f, c4Scr.y-7.f}, bombCol, bombBuf);
            }
        }

        // ==== WORLD LOOT ====
        if (worldLoot && g_masterEnabled) {
            for (int i = 0; i < slowLocal.itemCount; i++) {
                const WorldItem& it = slowLocal.items[i];
                if (!it.valid) continue;
                float dist = snap.localOrigin.DistTo(it.position);
                if (dist > 1500.0f) continue; // only show within 1500 units
                Vec2 scr;
                if (!WorldToScreen(it.position, snap.viewMatrix, scr)) continue;
                char lootBuf[48]; snprintf(lootBuf, sizeof(lootBuf), "%s (%.0f)", it.name, dist * 0.0254f);
                ImVec2 tsz = ImGui::CalcTextSize(lootBuf);
                drawList->AddText({scr.x - tsz.x/2.f + 1, scr.y + 1}, IM_COL32(0,0,0,200), lootBuf);
                drawList->AddText({scr.x - tsz.x/2.f,     scr.y},     IM_COL32(200,230,255,220), lootBuf);
            }
        }

        // ==== SPECTATOR LIST ====
        if (showSpectators && g_masterEnabled && slowLocal.spectatorCount > 0) {
            float sx = 12.f, sy = screenHeight * 0.5f;
            drawList->AddRectFilled({sx-4,sy-4},{sx+160, sy + slowLocal.spectatorCount*18.f + 4}, IM_COL32(0,0,0,160), 4.f);
            drawList->AddText({sx, sy-2}, IM_COL32(255,80,80,255), "Spectators:");
            sy += 16.f;
            for (int i = 0; i < slowLocal.spectatorCount; i++) {
                drawList->AddText({sx+1,sy+1}, IM_COL32(0,0,0,200), slowLocal.spectators[i].name);
                drawList->AddText({sx,  sy},   IM_COL32(255,200,200,230), slowLocal.spectators[i].name);
                sy += 17.f;
            }
        }

        // ==== IMGUI MENU ====
        if (menuOpen) {
            ImGuiContext& g = *GImGui;
            ImGuiStyle* style = &ImGui::GetStyle();

            // Watermark
            ImGui::SetNextWindowSize({410,50}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos({10,10},   ImGuiCond_FirstUseEver);
            ImGui::Begin("##watermark", nullptr,
                ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoCollapse|
                ImGuiWindowFlags_NoMouseInputs|ImGuiWindowFlags_NoNav);
            {
                const auto& p = ImGui::GetWindowPos();
                ImGui::GetWindowDrawList()->AddText(Inter_S,17.f,{p.x+16,p.y+16},ImGui::GetColorU32(c::main_color),"TRUST");
                ImGui::GetWindowDrawList()->AddText(Inter_S,17.f,{p.x+65,p.y+16},ImGui::GetColorU32(c::text_active),"CODE");
                if (image::user) ImGui::GetWindowDrawList()->AddImage(image::user,{p.x+120,p.y+15},{p.x+140,p.y+35},{},{1,1},ImGui::GetColorU32(c::main_color));
                char tgt[16]; sprintf(tgt,"%d TGT",telemetryTargets.load());
                ImGui::GetWindowDrawList()->AddText(Inter_S,17.f,{p.x+144,p.y+16},ImGui::GetColorU32(c::text_active),tgt);
                if (image::fps) ImGui::GetWindowDrawList()->AddImage(image::fps,{p.x+220,p.y+15},{p.x+240,p.y+35},{},{1,1},ImGui::GetColorU32(c::main_color));
                char fpsStr[16]; sprintf(fpsStr,"%d",telemetryFps.load());
                ImGui::GetWindowDrawList()->AddText(Inter_S,17.f,{p.x+244,p.y+16},ImGui::GetColorU32(c::text_active),fpsStr);
                ImGui::GetWindowDrawList()->AddText(Inter_S_3,15.f,{p.x+273,p.y+18},ImGui::GetColorU32(c::text_in_active),"FPS");
                if (image::ms) ImGui::GetWindowDrawList()->AddImage(image::ms,{p.x+310,p.y+15},{p.x+330,p.y+35},{},{1,1},ImGui::GetColorU32(c::main_color));
                char msStr[16]; sprintf(msStr,"%.2f",telemetryScanMs.load());
                ImGui::GetWindowDrawList()->AddText(Inter_S,17.f,{p.x+334,p.y+16},ImGui::GetColorU32(c::text_active),msStr);
                ImGui::GetWindowDrawList()->AddText(Inter_S_3,15.f,{p.x+360,p.y+18},ImGui::GetColorU32(c::text_in_active),"MS");
            }
            ImGui::End();

            // Main panel
            ImGui::SetNextWindowSize({1050,650}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSizeConstraints({650,420},{2560,1440});
            ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoCollapse);
            {
                const auto& p = ImGui::GetWindowPos();
                const ImVec2& region = ImGui::GetContentRegionMax();

                // Drag bar
                ImGui::SetCursorPos({0,0});
                ImGui::InvisibleButton("##hdr", {region.x, 40.f});
                if (ImGui::IsItemActive()) {
                    ImVec2 d = io.MouseDelta;
                    if (d.x || d.y) ImGui::SetWindowPos({p.x+d.x, p.y+d.y});
                }

                float cW   = std::max(400.f, region.x - 266.f);
                float colW = (cW - 20.f) / 2.f;
                float c1H  = std::max(120.f, (region.y - 95.f) * 0.40f);
                float c2H  = std::max(160.f, (region.y - 95.f) * 0.55f);

                ImGui::GetWindowDrawList()->AddRectFilled({p.x,p.y},{p.x+250,p.y+region.y},ImGui::GetColorU32(c::child_rect),12.f,ImDrawFlags_RoundCornersLeft);
                ImGui::GetWindowDrawList()->AddText(Inter_B,34.f,{p.x+27,p.y+32},ImGui::GetColorU32(c::main_color),"TRUST");
                ImGui::GetWindowDrawList()->AddText(Inter_B,34.f,{p.x+125,p.y+32},ImGui::GetColorU32(c::text_active),"CODE");

                // Tabs
                ImGui::SetCursorPos({8,112}); ImGui::BeginGroup();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,{0,8});
                if (ImGui::Tab("AimBot",   "A", 0==page, {234,50})) { page=0; active_tab=0; }
                if (ImGui::Tab("LegitBot", "L", 1==page, {234,50})) { page=1; active_tab=1; }
                if (ImGui::Tab("Visuals",  "V", 2==page, {234,50})) { page=2; active_tab=2; }
                if (ImGui::Tab("Settings", "S", 3==page, {234,50})) { page=3; active_tab=3; }
                if (ImGui::Tab("Config",   "C", 4==page, {234,50})) { page=4; active_tab=4; }
                ImGui::PopStyleVar(); ImGui::EndGroup();

                // ---- TAB 0: AIMBOT ----
                if (active_tab == 0) {
                    ImGui::GetWindowDrawList()->AddText(Inter_S_2,23.f,{p.x+266,p.y+18},ImGui::GetColorU32(c::text_active),"[AimBot]");

                    ImGui::SetCursorPos({266,76}); ImGui::BeginChild("tab0_1",{colW,c1H},false);
                    {
                        ImGui::Checkbox("Enable Aimbot", &aimbotEnabled);
                        ImGui::Checkbox("Master Enable (HOME)", &g_masterEnabled);
                        const char* keys[] = {"Left Alt","Right Alt","Left Shift","Right Shift"};
                        ImGui::Combo("Aim Key", &aimbotKey, keys, 4);
                        const char* boneOpts[] = {"Head","Neck","Chest"};
                        ImGui::Combo("Aim Bone", &aimbotBone, boneOpts, 3);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({266, 86+c1H}); ImGui::BeginChild("tab0_2",{colW,c2H},false);
                    {
                        ImGui::SliderFloat("FOV (deg)", &aimbotFovDeg,  0.5f, 30.f,  "%.1f");
                        ImGui::SliderFloat("Smooth",    &aimbotSmooth,  1.0f, 20.f,  "%.1f");
                        ImGui::Checkbox("Show FOV Circle", &aimbotFovCircle);
                        ImGui::Checkbox("Velocity Lead",   &aimbotLead);
                        ImGui::Checkbox("Wall Check (spotted)", &aimbotWallCheck);
                        ImGui::Separator();
                        ImGui::Checkbox("Bullet Tracer", &bulletTracer);
                        ImGui::ColorEdit4("Tracer Color", bulletColor, ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar);
                        ImGui::SliderInt("Radar Dist", &radarDistance, 1, 240);
                    }
                    ImGui::EndChild();

                    // Right col — status readout
                    ImGui::SetCursorPos({276+colW, 76}); ImGui::BeginChild("tab0_3",{colW,c1H},false);
                    {
                        ImGui::TextColored(g_masterEnabled ? ImVec4(0,1,.5f,1) : ImVec4(1,.3f,.3f,1),
                            g_masterEnabled ? "MASTER: ON" : "MASTER: OFF (HOME)");
                        ImGui::TextColored(aimbotEnabled ? ImVec4(0,1,.5f,1) : ImVec4(.6f,.6f,.6f,1),
                            aimbotEnabled ? "Aimbot: ACTIVE" : "Aimbot: OFF");
                    }
                    ImGui::EndChild();
                }

                // ---- TAB 1: LEGITBOT ----
                if (active_tab == 1) {
                    ImGui::GetWindowDrawList()->AddText(Inter_S_2,23.f,{p.x+266,p.y+18},ImGui::GetColorU32(c::text_active),"[LegitBot]");

                    ImGui::SetCursorPos({266,76}); ImGui::BeginChild("tab1_1",{colW,c2H},false);
                    {
                        ImGui::Checkbox("Enable Triggerbot", &triggerbotEnabled);
                        const char* modes[] = {"Hold Caps Lock","Always On"};
                        ImGui::Combo("Trigger Mode", &triggerMode, modes, 2);
                        ImGui::SliderInt("Trigger Delay (ms)", &triggerDelay, 0, 150);
                        ImGui::Separator();
                        ImGui::TextDisabled("Crosshair radius: 18px");
                        ImGui::TextDisabled("Checks: head + chest bones");
                        ImGui::TextDisabled("Output: SendInput LMB click");
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({276+colW,76}); ImGui::BeginChild("tab1_2",{colW,c2H},false);
                    {
                        ImGui::TextColored(triggerbotEnabled ? ImVec4(0,1,.5f,1) : ImVec4(.6f,.6f,.6f,1),
                            triggerbotEnabled ? "Triggerbot: ACTIVE" : "Triggerbot: OFF");
                        ImGui::Separator();
                        ImGui::Checkbox("Chams (Glow)", &chamsEnabled);
                        ImGui::ColorEdit4("Chams Color", chamsColor, ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar);
                    }
                    ImGui::EndChild();
                }

                // ---- TAB 2: VISUALS ----
                if (active_tab == 2) {
                    ImGui::GetWindowDrawList()->AddText(Inter_S_2,23.f,{p.x+266,p.y+18},ImGui::GetColorU32(c::text_active),"[Visuals]");

                    ImGui::SetCursorPos({266,76}); ImGui::BeginChild("tab2_1",{colW,c1H},false);
                    {
                        ImGui::Checkbox("Master ESP",  &espEnabled);
                        ImGui::Checkbox("Box ESP",     &drawBoxes);
                        const char* bst[] = {"Corner Brackets","Full Box"};
                        ImGui::Combo("Box Style", &boxStyle, bst, 2);
                        ImGui::Checkbox("Skeleton",    &drawSkeletons);
                        ImGui::Checkbox("Head Point",  &drawHeadDot);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({266, 86+c1H}); ImGui::BeginChild("tab2_2",{colW,c2H},false);
                    {
                        ImGui::Checkbox("Health Bar",  &drawHealth);
                        ImGui::Checkbox("Armor Bar",   &drawArmor);
                        ImGui::Checkbox("Player Name", &drawNames);
                        ImGui::Checkbox("Distance",    &drawDistance);
                        ImGui::Checkbox("Weapon Name", &drawWeapon);
                        ImGui::Checkbox("Eye Ray",     &drawEyeRay);
                        ImGui::Checkbox("Snaplines",   &drawSnaplines);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({276+colW,76}); ImGui::BeginChild("tab2_3",{colW,c1H},false);
                    {
                        ImGui::Checkbox("Enemy Only",  &teamCheck);
                        const char* themes[] = {"Cyber Violet","Electric Cyan","Emerald Green","Crimson Red"};
                        ImGui::Combo("Color Theme", &colorPalette, themes, 4);
                        ImGui::Separator();
                        ImGui::Checkbox("Sound ESP",   &soundEsp);
                        ImGui::Checkbox("Bomb ESP",    &bombEsp);
                        ImGui::Checkbox("World Loot",  &worldLoot);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({276+colW, 86+c1H}); ImGui::BeginChild("tab2_4",{colW,c2H},false);
                    {
                        ImGui::Checkbox("Spectators",  &showSpectators);
                        if (showSpectators && slowLocal.spectatorCount > 0) {
                            ImGui::Separator();
                            ImGui::TextColored({1,.4f,.4f,1},"Watching you:");
                            for (int i = 0; i < slowLocal.spectatorCount; i++)
                                ImGui::Text("  %s", slowLocal.spectators[i].name);
                        }
                    }
                    ImGui::EndChild();
                }

                // ---- TAB 3: SETTINGS ----
                if (active_tab == 3) {
                    ImGui::GetWindowDrawList()->AddText(Inter_S_2,23.f,{p.x+266,p.y+18},ImGui::GetColorU32(c::text_active),"[Settings]");
                    ImGui::SetCursorPos({266,76}); ImGui::BeginChild("tab3_1",{cW,c2H},false);
                    {
                        ImGui::Text("Menu Toggle : INSERT");
                        ImGui::Text("Panic Key   : HOME (toggle all features)");
                        ImGui::Text("Triggerbot  : Caps Lock (key-hold mode)");
                        ImGui::Separator();
                        ImGui::Text("CS2 PID     : %d",   pid);
                        ImGui::Text("Client Base : 0x%llX", (unsigned long long)clientBase);
                        ImGui::Text("Targets     : %d",   telemetryTargets.load());
                        ImGui::Text("Scan MS     : %.2f", telemetryScanMs.load());
                        ImGui::Text("Master      : %s",   g_masterEnabled ? "ON" : "OFF");
                        ImGui::Text("Timer Res   : 1ms (timeBeginPeriod)");
                        if (slowLocal.bomb.valid)
                            ImGui::Text("Bomb site   : %s", slowLocal.bomb.site==0?"A":(slowLocal.bomb.site==1?"B":"?"));
                    }
                    ImGui::EndChild();
                }

                // ---- TAB 4: CONFIG ----
                if (active_tab == 4) {
                    ImGui::GetWindowDrawList()->AddText(Inter_S_2,23.f,{p.x+266,p.y+18},ImGui::GetColorU32(c::text_active),"[Config]");
                    ImGui::SetCursorPos({266,76}); ImGui::BeginChild("tab4_1",{cW,c2H},false);
                    {
                        if (ImGui::Button("Save Preset Default",{200,35})) { /* TODO: json save */ }
                        if (ImGui::Button("Load Preset Default",{200,35})) { /* TODO: json load */ }
                        ImGui::Separator();
                        ImGui::TextDisabled("CSGO_Public-main feature set:");
                        ImGui::TextDisabled("  Aimbot     : velocity lead, per-bone");
                        ImGui::TextDisabled("  Triggerbot : proximity, bone-check");
                        ImGui::TextDisabled("  Sound ESP  : audio rings (2.5s window)");
                        ImGui::TextDisabled("  Bomb ESP   : countdown + defuse bar");
                        ImGui::TextDisabled("  Loot ESP   : weapon entities (500ms)");
                        ImGui::TextDisabled("  Spectators : observer services walk");
                        ImGui::TextDisabled("  Timer guard: 1ms scheduler quantum");
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::End();
        } // menuOpen

        // Render frame
        ImGui::Render();
        const float clr[4] = {0,0,0,0};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(0, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    running = false;
    if (memThread.joinable())  memThread.join();
    if (slowThread.joinable()) slowThread.join();

    DeleteCriticalSection(&g_snapLock);
    DeleteCriticalSection(&g_slowLock);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwndOverlay);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    if (hGame) CloseHandle(hGame);
    return 0;
}
