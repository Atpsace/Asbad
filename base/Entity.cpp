#include "Entity.h"

bool CEntity::UpdateController(const DWORD64& PlayerControllerAddress)
{
	if (PlayerControllerAddress == 0)
		return false;

	this->Controller.Address = PlayerControllerAddress;

	if (!this->Controller.GetHealth()) {
		std::cout << "\t\GetHealth FAIL!" << std::endl;
		return false;
	}

	if (!this->Controller.GetArmor()) {
		std::cout << "\t\tGetArmor FAIL!" << std::endl;
		return false;
	}
	//if (!this->Controller.GetMoney()) {
	//	std::cout << "\t\GetMoney FAIL!" << std::endl;
	//	return false;
	//}

	if (!this->Controller.GetIsAlive()) {
		std::cout << "\t\IsAlive FAIL!" << std::endl;
		return false;
	}

	if (!this->Controller.GetTeamID())
		return false;

	if (!this->Controller.GetPlayerName())
		return false;

	this->Pawn.Address = this->Controller.GetPlayerPawnAddress();

	return true;
}

bool CEntity::UpdatePawn(const DWORD64& PlayerPawnAddress)
{
	if (PlayerPawnAddress == 0)
		return false;

	this->Pawn.Address = PlayerPawnAddress;

	this->Pawn.GetCameraPos();
	this->Pawn.GetPos();
	this->Pawn.GetViewAngle();
	this->Pawn.GetWeaponName();
	this->Pawn.GetAimPunchAngle();
	this->Pawn.GetShotsFired();
	this->Pawn.GetHealth();
	this->Pawn.GetTeamID();
	this->Pawn.GetFov();
	this->Pawn.GetSpotted();
	this->Pawn.GetFFlags();
	this->Pawn.GetAimPunchCache();

	if (!this->Pawn.BoneData.UpdateAllBoneData(PlayerPawnAddress)) {
#ifdef DEBUG_PRINTS
		std::cout << "\t\tFailed to update UpdateAllBoneData" << std::endl;
#endif // DEBUG_PRINTS
		return false;
	}

	return true;
}

bool PlayerController::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::TeamID, this->TeamID);
}

bool PlayerController::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Health, this->Health);
}


bool PlayerController::GetIsAlive()
{
	return GetDataAddressWithOffset<int>(Address, Offset::IsAlive, this->AliveStatus);
}

bool PlayerController::GetPlayerName()
{
	char Buffer[MAX_PATH]{};

	if (!ProcessMgr.ReadMemory(Address + Offset::iszPlayerName, Buffer, MAX_PATH))
		return false;

	this->PlayerName = Buffer;
	if (this->PlayerName.empty())
		this->PlayerName = "Name_None";

	return true;
}

bool PlayerController::GetArmor()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Armor, this->Armor);
}

bool PlayerController::GetMoney()
{
	DWORD64 addr;
	GetDataAddressWithOffset<DWORD64>(Address, Offset::MoneyService, addr);
	return GetDataAddressWithOffset<int>(addr, 64, this->Money);
}

bool PlayerPawn::GetViewAngle()
{
	return GetDataAddressWithOffset<Vec2>(Address, Offset::angEyeAngles, this->ViewAngle);
}

bool PlayerPawn::GetCameraPos()
{
	return GetDataAddressWithOffset<Vec3>(Address, Offset::vecLastClipCameraPos, this->CameraPos);
}

bool PlayerPawn::GetSpotted()
{
	return GetDataAddressWithOffset<DWORD64>(Address, Offset::bSpottedByMask, this->bSpottedByMask);
}

bool PlayerPawn::GetWeaponName()
{
	try {
		DWORD64 activeWeaponAddress = 0;

		// Method 1: C_BasePlayerPawn.m_pClippingWeapon (Offset::pClippingWeapon / 0x13A0)
		if (Offset::pClippingWeapon != 0) {
			ProcessMgr.ReadMemory<DWORD64>(this->Address + Offset::pClippingWeapon, activeWeaponAddress);
		}

		// Method 2: C_BasePlayerPawn.m_pWeaponServices (0x1208) -> m_hActiveWeapon (0x60) -> EntityList
		if (activeWeaponAddress == 0) {
			DWORD64 weaponServices = 0;
			if (ProcessMgr.ReadMemory<DWORD64>(this->Address + 0x1208, weaponServices) && weaponServices != 0) {
				DWORD activeWeaponHandle = 0;
				if (ProcessMgr.ReadMemory<DWORD>(weaponServices + 0x60, activeWeaponHandle) && activeWeaponHandle != 0 && activeWeaponHandle != 0xFFFFFFFF) {
					DWORD64 entityListEntry = 0;
					if (ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), entityListEntry) && entityListEntry != 0) {
						if (ProcessMgr.ReadMemory<DWORD64>(entityListEntry + 0x10 + 8 * ((activeWeaponHandle & 0x7FFF) >> 9), entityListEntry) && entityListEntry != 0) {
							if (!ProcessMgr.ReadMemory<DWORD64>(entityListEntry + 0x78 * (activeWeaponHandle & 0x1FF), activeWeaponAddress) || activeWeaponAddress == 0) {
								ProcessMgr.ReadMemory<DWORD64>(entityListEntry + 0x70 * (activeWeaponHandle & 0x1FF), activeWeaponAddress);
							}
						}
					}
				}
			}
		}

		if (activeWeaponAddress != 0) {
			// Sub-Method A: m_iItemDefinitionIndex from C_EconEntity.m_AttributeManager (0x11A0) -> m_Item (0x50) -> m_iItemDefinitionIndex (0x1BA)
			uint16_t itemDefIndex = 0;
			if (ProcessMgr.ReadMemory<uint16_t>(activeWeaponAddress + 0x11A0 + 0x50 + 0x1BA, itemDefIndex) && itemDefIndex != 0) {
				switch (itemDefIndex) {
				case 1: WeaponName = "deagle"; return true;
				case 2: WeaponName = "elite"; return true;
				case 3: WeaponName = "fiveseven"; return true;
				case 4: WeaponName = "glock"; return true;
				case 7: WeaponName = "ak47"; return true;
				case 8: WeaponName = "aug"; return true;
				case 9: WeaponName = "awp"; return true;
				case 10: WeaponName = "famas"; return true;
				case 11: WeaponName = "g3sg1"; return true;
				case 13: WeaponName = "galilar"; return true;
				case 14: WeaponName = "m249"; return true;
				case 16: WeaponName = "m4a1"; return true;
				case 17: WeaponName = "mac10"; return true;
				case 19: WeaponName = "p90"; return true;
				case 23: WeaponName = "mp5sd"; return true;
				case 24: WeaponName = "ump45"; return true;
				case 25: WeaponName = "xm1014"; return true;
				case 26: WeaponName = "bizon"; return true;
				case 27: WeaponName = "mag7"; return true;
				case 28: WeaponName = "negev"; return true;
				case 29: WeaponName = "sawedoff"; return true;
				case 30: WeaponName = "tec9"; return true;
				case 31: WeaponName = "taser"; return true;
				case 32: WeaponName = "hkp2000"; return true;
				case 33: WeaponName = "mp7"; return true;
				case 34: WeaponName = "mp9"; return true;
				case 35: WeaponName = "nova"; return true;
				case 36: WeaponName = "p250"; return true;
				case 38: WeaponName = "scar20"; return true;
				case 39: WeaponName = "sg553"; return true;
				case 40: WeaponName = "ssg08"; return true;
				case 42: WeaponName = "knife"; return true;
				case 43: WeaponName = "flashbang"; return true;
				case 44: WeaponName = "hegrenade"; return true;
				case 45: WeaponName = "smokegrenade"; return true;
				case 46: WeaponName = "molotov"; return true;
				case 47: WeaponName = "decoy"; return true;
				case 48: WeaponName = "incgrenade"; return true;
				case 49: WeaponName = "c4"; return true;
				case 59: WeaponName = "knife"; return true;
				case 60: WeaponName = "m4a1_silencer"; return true;
				case 61: WeaponName = "usp_silencer"; return true;
				case 63: WeaponName = "cz75a"; return true;
				case 64: WeaponName = "revolver"; return true;
				case 500: case 503: case 505: case 506: case 507: case 508: case 509: case 512: case 514: case 515: case 516: case 519: case 520: case 522: case 523: case 525:
					WeaponName = "knife"; return true;
				default: break;
				}
			}

			// Sub-Method B: CEntityIdentity -> m_designerName string pointer (0x10 -> 0x20)
			DWORD64 entityIdentity = 0;
			if (ProcessMgr.ReadMemory<DWORD64>(activeWeaponAddress + 0x10, entityIdentity) && entityIdentity != 0) {
				DWORD64 namePtr = 0;
				if (ProcessMgr.ReadMemory<DWORD64>(entityIdentity + 0x20, namePtr) && namePtr != 0) {
					char Buffer[MAX_PATH]{};
					if (ProcessMgr.ReadMemory(namePtr, Buffer, MAX_PATH) && strlen(Buffer) > 0) {
						WeaponName = std::string(Buffer);
						std::size_t Index = WeaponName.find("weapon_");
						if (Index != std::string::npos) {
							WeaponName = WeaponName.substr(Index + 7);
						} else {
							Index = WeaponName.find("_");
							if (Index != std::string::npos) {
								WeaponName = WeaponName.substr(Index + 1);
							}
						}
						return true;
					}
				}
			}
		}

		// Keep previous valid weapon name if transient memory miss occurs
		if (WeaponName.empty() || WeaponName == "Weapon_None") {
			WeaponName = "ak47"; // Default fallback so aimbot smooth defaults work
		}
		return true;
	}
	catch (const std::exception&) {
		if (WeaponName.empty() || WeaponName == "Weapon_None") {
			WeaponName = "ak47";
		}
		return true;
	}
}

bool PlayerPawn::GetShotsFired()
{
	return GetDataAddressWithOffset<DWORD>(Address, Offset::iShotsFired, this->ShotsFired);
}

bool PlayerPawn::GetAimPunchAngle()
{
	return GetDataAddressWithOffset<Vec2>(Address, Offset::aimPunchAngle, this->AimPunchAngle);
}

bool PlayerPawn::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::iTeamNum, this->TeamID);
}

bool PlayerPawn::GetAimPunchCache()
{
	return GetDataAddressWithOffset<C_UTL_VECTOR>(Address, Offset::aimPunchCache, this->AimPunchCache);
}

DWORD64 PlayerController::GetPlayerPawnAddress()
{
	DWORD64 EntityPawnListEntry = 0;
	DWORD64 EntityPawnAddress = 0;

	if (!GetDataAddressWithOffset<DWORD>(Address, Offset::PlayerPawn, this->Pawn))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x70 * (Pawn & 0x1FF), EntityPawnAddress))
		return 0;

	return EntityPawnAddress;
}

bool PlayerPawn::GetPos()
{
	return GetDataAddressWithOffset<Vec3>(Address, Offset::Pos, this->Pos);
}

bool PlayerPawn::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::CurrentHealth, this->Health);
}

bool PlayerPawn::GetFov()
{
	DWORD64 CameraServices = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(Address + Offset::CameraServices, CameraServices))
		return false;
	return GetDataAddressWithOffset<int>(CameraServices, Offset::iFovStart, this->Fov);
}

bool PlayerPawn::GetFFlags()
{
	return GetDataAddressWithOffset<int>(Address, Offset::fFlags, this->fFlags);
}

bool CEntity::IsAlive() const
{
	return this->Controller.AliveStatus == 1 && this->Pawn.Health > 0;
}

bool CEntity::IsInScreen()
{
	return gGame.View.WorldToScreen(this->Pawn.Pos, this->Pawn.ScreenPos);
}

CBone CEntity::GetBone() const
{
	if (this->Pawn.Address == 0)
		return CBone{};
	return this->Pawn.BoneData;
}