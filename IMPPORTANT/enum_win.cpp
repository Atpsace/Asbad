#include <windows.h>
#include <cstdio>

BOOL CALLBACK EnumWin(HWND hwnd, LPARAM lParam) {
  char title[256] = {0};
  char cls[256] = {0};
  GetWindowTextA(hwnd, title, sizeof(title));
  GetClassNameA(hwnd, cls, sizeof(cls));
  DWORD pid = 0;
  GetWindowThreadProcessId(hwnd, &pid);
  if (strlen(title) > 0 && IsWindowVisible(hwnd)) {
    printf("HWND=0x%p PID=%d Cls='%s' Title='%s'\n", hwnd, pid, cls, title);
  }
  return TRUE;
}

int main() {
  EnumWindows(EnumWin, 0);
  return 0;
}
