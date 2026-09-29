#include "Offsets.h"

#include "rapidjson/document.h"
#include <iostream>
#include <iomanip>

using namespace rapidjson;

bool Offset::UpdateOffsets(std::string offsetdata, std::string clientdata)
{
	Document offsets, client;
	offsets.Parse(offsetdata.c_str());
	client.Parse(clientdata.c_str());

	offsetdata.clear(); clientdata.clear();

	auto get_client_field = [&](const char* className, const char* fieldName) -> uint64_t {
		if (client.HasMember("client.dll") && client["client.dll"].HasMember("classes") &&
			client["client.dll"]["classes"].HasMember(className) &&
			client["client.dll"]["classes"][className].HasMember("fields") &&
			client["client.dll"]["classes"][className]["fields"].HasMember(fieldName)) {
			const auto& f = client["client.dll"]["classes"][className]["fields"][fieldName];
			if (f.IsObject() && f.HasMember("offset") && f["offset"].IsUint64()) {
				return f["offset"].GetUint64();
			}
			if (f.IsUint64()) {
				return f.GetUint64();
			}
		}
		return 0;
	};

	Offset::EntityList = offsets["client.dll"]["dwEntityList"].GetUint64();
	Offset::Matrix = offsets["client.dll"]["dwViewMatrix"].GetUint64();
	Offset::ViewAngle = offsets["client.dll"]["dwViewAngles"].GetUint64();
	Offset::LocalPlayerController = offsets["client.dll"]["dwLocalPlayerController"].GetUint64();
	Offset::LocalPlayerPawn = offsets["client.dll"]["dwLocalPlayerPawn"].GetUint64();
	Offset::GlobalVars = offsets["client.dll"]["dwGlobalVars"].GetUint64();

	Offset::Health = get_client_field("C_BaseEntity", "m_iHealth");
	Offset::TeamID = get_client_field("C_BaseEntity", "m_iTeamNum");
	Offset::Armor = get_client_field("CCSPlayerController", "m_iPawnArmor");
	Offset::IsAlive = get_client_field("CCSPlayerController", "m_bPawnIsAlive");
	Offset::MoneyService = get_client_field("CCSPlayerController", "m_pInGameMoneyServices");
	Offset::PlayerPawn = get_client_field("CCSPlayerController", "m_hPlayerPawn");
	Offset::iszPlayerName = get_client_field("CBasePlayerController", "m_iszPlayerName");
	Offset::Pos = get_client_field("C_BasePlayerPawn", "m_vOldOrigin");
	Offset::MaxHealth = get_client_field("C_BaseEntity", "m_iMaxHealth");
	Offset::CurrentHealth = get_client_field("C_BaseEntity", "m_iHealth");
	Offset::GameSceneNode = get_client_field("C_BaseEntity", "m_pGameSceneNode");
	Offset::BoneArray = get_client_field("CSkeletonInstance", "m_modelState") + 0x80;
	
	if (offsets.HasMember("matchmaking.dll") && offsets["matchmaking.dll"].HasMember("dwGameTypes_mapName")) {
		Offset::MapName = offsets["matchmaking.dll"]["dwGameTypes_mapName"].GetUint64();
	} else if (offsets.HasMember("matchmaking.dll") && offsets["matchmaking.dll"].HasMember("dwGameTypes")) {
		Offset::MapName = offsets["matchmaking.dll"]["dwGameTypes"].GetUint64();
	} else {
		Offset::MapName = 0;
	}

	Offset::angEyeAngles = get_client_field("C_CSPlayerPawn", "m_angEyeAngles");
	Offset::vecLastClipCameraPos = get_client_field("C_CSPlayerPawn", "m_vecLastClipCameraPos");
	Offset::pClippingWeapon = get_client_field("C_CSPlayerPawn", "m_pClippingWeapon");
	Offset::iShotsFired = get_client_field("C_CSPlayerPawn", "m_iShotsFired");
	Offset::flFlashDuration = get_client_field("C_CSPlayerPawnBase", "m_flFlashDuration");
	Offset::aimPunchAngle = get_client_field("C_CSPlayerPawn", "m_aimPunchAngle");
	Offset::aimPunchCache = get_client_field("C_CSPlayerPawn", "m_aimPunchCache");
	Offset::iIDEntIndex = get_client_field("C_CSPlayerPawn", "m_iIDEntIndex");
	Offset::iTeamNum = get_client_field("C_BaseEntity", "m_iTeamNum");
	Offset::CameraServices = get_client_field("C_BasePlayerPawn", "m_pCameraServices");
	Offset::iFovStart = get_client_field("CCSPlayerBase_CameraServices", "m_iFOVStart");
	Offset::fFlags = get_client_field("C_BaseEntity", "m_fFlags");

	int m_entitySpottedState = get_client_field("C_CSPlayerPawn", "m_entitySpottedState");
	int m_bSpottedByMask = get_client_field("EntitySpottedState_t", "m_bSpottedByMask");
	Offset::bSpottedByMask = m_entitySpottedState + m_bSpottedByMask;

	//auto print_hex = [](const char* name, uint64_t value) {
	//	std::cout << std::left << std::setw(30) << name << " : 0x" << std::hex << value << std::dec << std::endl;
	//};
	//auto print_int = [](const char* name, int value) {
	//	std::cout << std::left << std::setw(30) << name << " : " << value << std::endl;
	//};

	//print_hex("EntityList", Offset::EntityList);
	//print_hex("Matrix", Offset::Matrix);
	//print_hex("ViewAngle", Offset::ViewAngle);
	//print_hex("LocalPlayerController", Offset::LocalPlayerController);
	//print_hex("LocalPlayerPawn", Offset::LocalPlayerPawn);
	//print_hex("GlobalVars", Offset::GlobalVars);
	//print_hex("Health", Offset::Health);
	//print_hex("TeamID", Offset::TeamID);
	//print_hex("Armor", Offset::Armor);
	//print_hex("IsAlive", Offset::IsAlive);
	//print_hex("MoneyService", Offset::MoneyService);
	//print_hex("PlayerPawn", Offset::PlayerPawn);
	//print_hex("iszPlayerName", Offset::iszPlayerName);
	//print_hex("Pos", Offset::Pos);
	//print_hex("MaxHealth", Offset::MaxHealth);
	//print_hex("CurrentHealth", Offset::CurrentHealth);
	//print_hex("GameSceneNode", Offset::GameSceneNode);
	//print_hex("BoneArray", Offset::BoneArray);
	//print_hex("MapName", Offset::MapName);
	//print_hex("angEyeAngles", Offset::angEyeAngles);
	//print_hex("vecLastClipCameraPos", Offset::vecLastClipCameraPos);
	//print_hex("pClippingWeapon", Offset::pClippingWeapon);
	//print_hex("iShotsFired", Offset::iShotsFired);
	//print_hex("flFlashDuration", Offset::flFlashDuration);
	//print_hex("aimPunchAngle", Offset::aimPunchAngle);
	//print_hex("aimPunchCache", Offset::aimPunchCache);
	//print_hex("iIDEntIndex", Offset::iIDEntIndex);
	//print_hex("iTeamNum", Offset::iTeamNum);
	//print_hex("CameraServices", Offset::CameraServices);
	//print_hex("iFovStart", Offset::iFovStart);
	//print_hex("fFlags", Offset::fFlags);

	//print_int("m_entitySpottedState", m_entitySpottedState);
	//print_int("m_bSpottedByMask", m_bSpottedByMask);
	//print_hex("bSpottedByMask", Offset::bSpottedByMask);

	return true;
}