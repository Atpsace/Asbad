#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>

BOOL CALLBACK EnumThreadWnd(HWND hwnd, LPARAM lParam) {
  char title[256] = {0};
  char cls[256] = {0};
  GetWindowTextA(hwnd, title, sizeof(title));
  GetClassNameA(hwnd, cls, sizeof(cls));
  RECT r{};
  GetWindowRect(hwnd, &r);
  printf("  Thread HWND: 0x%p Class='%s' Title='%s' Rect=(%d, %d, %d, %d) Visible=%d\n",
         hwnd, cls, title, r.left, r.top, r.right, r.bottom, IsWindowVisible(hwnd));
  return TRUE;
}

int main() {
  DWORD cs2Pid = 12252;
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if (hSnap == INVALID_HANDLE_VALUE) return 1;
  THREADENTRY32 te = { sizeof(te) };
  if (Thread32First(hSnap, &te)) {
    do {
      if (te.th32OwnerProcessID == cs2Pid) {
        printf("Thread %d:\n", te.th32ThreadID);
        EnumThreadWindows(te.th32ThreadID, EnumThreadWnd, 0);
      }
    } while (Thread32Next(hSnap, &te));
  }
  CloseHandle(hSnap);
  return 0;
}
