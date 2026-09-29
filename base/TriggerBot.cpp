#include "TriggerBot.h"
#include "Cheats.h"
#include "../kmbox/Makcu.h"

void TriggerBot::Run(const CEntity& LocalEntity)
{
	int uHandle = 0;
	if (!ProcessMgr.ReadMemory<int>(LocalEntity.Pawn.Address + Offset::iIDEntIndex, uHandle))
		return;
	if (uHandle <= 0)
		return;

	CEntity TargetEntity;
	bool foundTarget = false;

	// Method 1: Direct EntityList matching by index / handle
	for (const auto& ent : Cheats::EntityList) {
		if (ent.Pawn.Address == 0 || !ent.IsAlive()) continue;
		if (ent.Pawn.Health <= 0) continue;

		// Match by index handle
		if ((ent.Controller.Pawn & 0x1FF) == (uHandle & 0x1FF) || (ent.LocalPlayerControllerIndex + 1) == uHandle) {
			TargetEntity = ent;
			foundTarget = true;
			break;
		}
	}

	// Method 2: Entity list lookup via entity list pointer
	if (!foundTarget) {
		DWORD64 ListEntry = 0;
		if (ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), ListEntry) && ListEntry != 0) {
			if (ProcessMgr.ReadMemory<DWORD64>(ListEntry + 0x8 * (uHandle >> 9) + 0x10, ListEntry) && ListEntry != 0) {
				DWORD64 PawnAddress = 0;
				if (!ProcessMgr.ReadMemory<DWORD64>(ListEntry + 0x78 * (uHandle & 0x1FF), PawnAddress) || PawnAddress == 0) {
					ProcessMgr.ReadMemory<DWORD64>(ListEntry + 0x70 * (uHandle & 0x1FF), PawnAddress);
				}
				if (PawnAddress != 0) {
					TargetEntity.UpdatePawn(PawnAddress);
					if (TargetEntity.Pawn.Health > 0) {
						foundTarget = true;
					}
				}
			}
		}
	}

	// Method 3: Fallback raycast screen center proximity check for any active player entity
	if (!foundTarget) {
		float bestScreenDist = 45.f; // Max crosshair radius for trigger
		for (const auto& ent : Cheats::EntityList) {
			if (!ent.IsAlive() || ent.Pawn.Health <= 0) continue;
			if (ent.GetBone().BonePosList.size() <= BONEINDEX::head) continue;

			Vec2 headScreen = ent.GetBone().BonePosList[BONEINDEX::head].ScreenPos;
			Vec2 chestScreen = (ent.GetBone().BonePosList.size() > BONEINDEX::spine_2) ? ent.GetBone().BonePosList[BONEINDEX::spine_2].ScreenPos : headScreen;

			float headDist = headScreen.DistanceTo({ (float)Gui.Window.Size.x / 2.f, (float)Gui.Window.Size.y / 2.f });
			float chestDist = chestScreen.DistanceTo({ (float)Gui.Window.Size.x / 2.f, (float)Gui.Window.Size.y / 2.f });

			float minDist = (headDist < chestDist) ? headDist : chestDist;
			if (minDist < bestScreenDist) {
				bestScreenDist = minDist;
				TargetEntity = ent;
				foundTarget = true;
			}
		}
	}

	if (!foundTarget)
		return;

	const CEntity& Entity = TargetEntity;

	bool AllowShoot = false;

	if (MenuConfig::TeamCheck)
		AllowShoot = LocalEntity.Pawn.TeamID != Entity.Pawn.TeamID && Entity.Pawn.Health > 0;
	else
		AllowShoot = Entity.Pawn.Health > 0;

	if (AllowShoot && TriggerBot::TargetBoneFilter > 0)
	{
		// Check specific bone target filter (1=Head, 2=Neck, 3=Chest, 4=Pelvis)
		DWORD reqBone = BONEINDEX::head;
		if (TriggerBot::TargetBoneFilter == 2) reqBone = BONEINDEX::neck_0;
		else if (TriggerBot::TargetBoneFilter == 3) reqBone = BONEINDEX::spine_2;
		else if (TriggerBot::TargetBoneFilter == 4) reqBone = BONEINDEX::pelvis;

		if (Entity.GetBone().BonePosList.size() > reqBone)
		{
			Vec2 screenPos = Entity.GetBone().BonePosList[reqBone].ScreenPos;
			float dist = screenPos.DistanceTo({ (float)Gui.Window.Size.x / 2.f, (float)Gui.Window.Size.y / 2.f });
			if (dist > 35.f) {
				AllowShoot = false;
			}
		}
	}

	if (!AllowShoot)
		return;

	static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
	auto CurTimePoint = std::chrono::steady_clock::now();
	if (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(TriggerDelay))
	{
		if (!Keys::LeftKey)
		{
			if (KmBox::type == "net") {
				KmBoxMgr.Mouse.Left(true);
				Sleep(10);
				KmBoxMgr.Mouse.Left(false);
			}
			else if (KmBox::type == "net2") {
				kmboxnew::kmNet_mask_mouse_left(true);
				Sleep(10);
				kmboxnew::kmNet_mask_mouse_left(false);
			}
			else if (KmBox::type == "b") {
				kmBoxBMgr.km_click();
			}
			else if (KmBox::type == "makcu") {
				makcuMgr.click();
			}
		}
		LastTimePoint = CurTimePoint;
	}
}