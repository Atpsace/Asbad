#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdint>

struct Vec3 { float x, y, z; };

DWORD GetProcessIdByName(const wchar_t* name) {
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnap == INVALID_HANDLE_VALUE) return 0;
  PROCESSENTRY32W pe = {sizeof(pe)};
  DWORD pid = 0;
  if (Process32FirstW(hSnap, &pe)) {
    do {
      if (wcsicmp(pe.szExeFile, name) == 0) {
        pid = pe.th32ProcessID;
        break;
      }
    } while (Process32NextW(hSnap, &pe));
  }
  CloseHandle(hSnap);
  return pid;
}

uintptr_t GetModuleBase(DWORD pid, const wchar_t* modName) {
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
  if (hSnap == INVALID_HANDLE_VALUE) return 0;
  MODULEENTRY32W me = {sizeof(me)};
  uintptr_t baseAddr = 0;
  if (Module32FirstW(hSnap, &me)) {
    do {
      if (wcsicmp(me.szModule, modName) == 0) {
        baseAddr = (uintptr_t)me.modBaseAddr;
        break;
      }
    } while (Module32NextW(hSnap, &me));
  }
  CloseHandle(hSnap);
  return baseAddr;
}

int main() {
  DWORD pid = GetProcessIdByName(L"cs2.exe");
  if (!pid) return 1;

  HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
  if (!hProc) return 1;

  uintptr_t clientBase = GetModuleBase(pid, L"client.dll");
  uintptr_t localPawn = 0;
  ReadProcessMemory(hProc, (LPCVOID)(clientBase + 0x23C6268), &localPawn, sizeof(localPawn), nullptr);
  int localTeam = 0;
  if (localPawn) {
    ReadProcessMemory(hProc, (LPCVOID)(localPawn + 0x3E7), &localTeam, sizeof(localTeam), nullptr);
  }
  printf("LocalPlayer: 0x%llX Team: %d\n", (unsigned long long)localPawn, localTeam & 0xFF);

  uintptr_t entityList = 0;
  ReadProcessMemory(hProc, (LPCVOID)(clientBase + 0x2571220), &entityList, sizeof(entityList), nullptr);
  uintptr_t listEntry = 0;
  ReadProcessMemory(hProc, (LPCVOID)(entityList + 0x10), &listEntry, sizeof(listEntry), nullptr);

  BYTE block[0x2000];
  ReadProcessMemory(hProc, (LPCVOID)listEntry, block, sizeof(block), nullptr);

  for (int i = 1; i < 64; i++) {
    uintptr_t ctrl = *(uintptr_t*)(block + (i * 0x70));
    if (!ctrl) ctrl = *(uintptr_t*)(block + (i * 0x78));
    if (!ctrl) continue;

    uint32_t pawnHandle = 0;
    ReadProcessMemory(hProc, (LPCVOID)(ctrl + 0x914), &pawnHandle, sizeof(pawnHandle), nullptr);
    if (!pawnHandle || pawnHandle == 0xFFFFFFFF) continue;

    uintptr_t listEntry2 = 0;
    uintptr_t pawnEntryOffset = entityList + 0x10 + 8 * ((pawnHandle & 0x7FFF) >> 9);
    ReadProcessMemory(hProc, (LPCVOID)pawnEntryOffset, &listEntry2, sizeof(listEntry2), nullptr);
    if (!listEntry2) continue;

    uintptr_t pawn = 0;
    ReadProcessMemory(hProc, (LPCVOID)(listEntry2 + 0x70 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
    if (!pawn) {
      ReadProcessMemory(hProc, (LPCVOID)(listEntry2 + 0x78 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
    }
    if (!pawn || pawn == localPawn) continue;

    int hp = 0, team = 0;
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x34C), &hp, sizeof(hp), nullptr);
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x3E7), &team, sizeof(team), nullptr);
    team &= 0xFF;

    Vec3 origin{};
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x13B8), &origin, sizeof(origin), nullptr);

    uintptr_t gameScene = 0;
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x330), &gameScene, sizeof(gameScene), nullptr);
    uintptr_t boneArray = 0;
    if (gameScene) {
      ReadProcessMemory(hProc, (LPCVOID)(gameScene + 0x1C0), &boneArray, sizeof(boneArray), nullptr);
    }

    char name[64] = {0};
    ReadProcessMemory(hProc, (LPCVOID)(ctrl + 0x6F4), name, sizeof(name) - 1, nullptr);

    printf("Slot %2d: Name='%s' HP=%d Team=%d Pos=(%.1f, %.1f, %.1f) Scene=0x%llX Bones=0x%llX\n",
      i, name, hp, team, origin.x, origin.y, origin.z, (unsigned long long)gameScene, (unsigned long long)boneArray);
  }

  CloseHandle(hProc);
  return 0;
}
