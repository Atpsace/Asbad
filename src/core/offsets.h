#pragma once
#include <cstdint>

namespace Offsets {

// Build 14185 from sezzyaep/CS2-OFFSETS
namespace Client {
    constexpr uintptr_t dwEntityList = 0x27151A8;
    constexpr uintptr_t dwLocalPlayerPawn = 0x25606D8;
    constexpr uintptr_t dwLocalPlayerController = 0x2537628;
    constexpr uintptr_t dwViewMatrix = 0x2565A20;
    constexpr uintptr_t dwPlantedC4 = 0x24C9290;
    constexpr uintptr_t dwWeaponC4 = 0x24C4550;
    constexpr uintptr_t dwGlobalVars = 0x222BF88;
    constexpr uintptr_t dwViewAngles = 0x2576238;
    constexpr uintptr_t dwSensitivity = 0x255D428;
}

namespace Pawn {
    constexpr uintptr_t m_iHealth = 0x34C;
    constexpr uintptr_t m_iTeamNum = 0x3E7;
    constexpr uintptr_t m_pGameSceneNode = 0x330;
    constexpr uintptr_t m_vecAbsOrigin = 0xC8;
    constexpr uintptr_t m_vOldOrigin = 0x14A4;
    constexpr uintptr_t m_vecViewOffset = 0xF60;
    constexpr uintptr_t m_absVelocity = 0x3F8;
    constexpr uintptr_t m_fFlags = 0x3F4;
    constexpr uintptr_t m_entitySpottedState = 0x1E88;
    constexpr uintptr_t m_emitSndTime = 0x1EA8;
    constexpr uintptr_t m_iShotsFired = 0x1EB4;
    constexpr uintptr_t m_angEyeAngles = 0x35F0;
    constexpr uintptr_t m_ArmorValue = 0x1ECC;
    constexpr uintptr_t m_iClip1 = 0x1928;
    constexpr uintptr_t m_pWeaponSvc = 0x12F0;
    constexpr uintptr_t m_hOwnerEntity = 0x520;
    constexpr uintptr_t m_aimPunchAngleDirect = 0x15B0;
    constexpr uintptr_t m_pAimPunchServices = 0x1598;
    constexpr uintptr_t m_pCameraServices = 0x1328;
    constexpr uintptr_t m_pObserverSvc = 0x1308;
}

namespace Controller {
    constexpr uintptr_t m_hPlayerPawn = 0x92C;
    constexpr uintptr_t m_bPawnIsAlive = 0x934;
    constexpr uintptr_t m_iszPlayerName = 0x6FC;
}

namespace SceneNode {
    constexpr uintptr_t m_modelState = 0x140;
    constexpr uintptr_t m_boneArray = 0x1C0;
}

namespace SpottedState {
    constexpr uintptr_t m_bSpotted = 0x8;
    constexpr uintptr_t m_bSpottedByMask = 0xC;
}

namespace WeaponSvc {
    constexpr uintptr_t m_hActiveWpn = 0x60;
}

namespace Weapon {
    constexpr uintptr_t m_AttrMgr = 0x1290;
    constexpr uintptr_t m_Item_off = 0x50;
    constexpr uintptr_t m_iItemDefIdx = 0x1BA;
    constexpr uintptr_t m_bPinPulled = 0x1F23;
    constexpr uintptr_t m_flThrowStrength = 0x1F30;
}

namespace C4 {
    constexpr uintptr_t c4_bTicking = 0x1288;
    constexpr uintptr_t c4_nSite = 0x128C;
    constexpr uintptr_t c4_flBlow = 0x12B8;
    constexpr uintptr_t c4_flTimer = 0x12C0;
    constexpr uintptr_t c4_bDefusing = 0x12C4;
    constexpr uintptr_t c4_flDefuseLen = 0x12D4;
    constexpr uintptr_t c4_flDefuseCountDown = 0x12D8;
    constexpr uintptr_t c4_bDefused = 0x12DC;
}

namespace Entity {
    constexpr uintptr_t m_pEntity_id = 0x10;
    constexpr uintptr_t m_designerName = 0x20;
}

namespace Observer {
    constexpr uintptr_t m_hObsTgt = 0x4C;
}

namespace Camera {
    constexpr uintptr_t m_vecCsViewPunchAngle = 0x48;
}

namespace AimPunch {
    constexpr uintptr_t m_predictableBaseAngle = 0x50;
}

namespace Sensitivity {
    constexpr uintptr_t dwSensitivity_sensitivity = 0x58;
}

} // namespace Offsets
