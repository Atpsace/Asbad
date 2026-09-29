#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdint>

struct Vec3 { float x, y, z; };
struct BoneJointData { Vec3 pos; char pad[0x14]; };

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
  if (!pid) { printf("CS2 not found\n"); return 1; }
  HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
  if (!hProc) { printf("OpenProcess failed\n"); return 1; }
  uintptr_t clientBase = GetModuleBase(pid, L"client.dll");

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

    uintptr_t pawnEntryOffset = entityList + 0x10 + 8 * ((pawnHandle & 0x7FFF) >> 9);
    uintptr_t listEntry2 = 0;
    ReadProcessMemory(hProc, (LPCVOID)pawnEntryOffset, &listEntry2, sizeof(listEntry2), nullptr);
    if (!listEntry2) continue;
    uintptr_t pawn = 0;
    ReadProcessMemory(hProc, (LPCVOID)(listEntry2 + 0x70 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
    if (!pawn) ReadProcessMemory(hProc, (LPCVOID)(listEntry2 + 0x78 * (pawnHandle & 0x1FF)), &pawn, sizeof(pawn), nullptr);
    if (!pawn) continue;

    int hp = 0;
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x34C), &hp, sizeof(hp), nullptr);
    if (hp <= 0) continue;

    Vec3 origin{};
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x13B8), &origin, sizeof(origin), nullptr);

    uintptr_t gameScene = 0;
    ReadProcessMemory(hProc, (LPCVOID)(pawn + 0x330), &gameScene, sizeof(gameScene), nullptr);
    uintptr_t boneArray = 0;
    if (gameScene) ReadProcessMemory(hProc, (LPCVOID)(gameScene + 0x1C0), &boneArray, sizeof(boneArray), nullptr);

    BoneJointData bones[30]{};
    if (boneArray) ReadProcessMemory(hProc, (LPCVOID)boneArray, bones, sizeof(bones), nullptr);

    char name[64] = {0};
    ReadProcessMemory(hProc, (LPCVOID)(ctrl + 0x6F4), name, sizeof(name) - 1, nullptr);

    printf("Slot %2d: %s | m_vOldOrigin=(%.1f, %.1f, %.1f) | bone0=(%.1f, %.1f, %.1f) | bone6=(%.1f, %.1f, %.1f) | bone7=(%.1f, %.1f, %.1f)\n",
           i, name, origin.x, origin.y, origin.z, bones[0].pos.x, bones[0].pos.y, bones[0].pos.z, bones[6].pos.x, bones[6].pos.y, bones[6].pos.z, bones[7].pos.x, bones[7].pos.y, bones[7].pos.z);
    break; // only need 1
  }
  CloseHandle(hProc);
  return 0;
}
