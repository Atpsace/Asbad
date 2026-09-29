#include <windows.h>
#include <cstdio>

int main() {
  HWINSTA hw = OpenWindowStationA("WinSta0", FALSE, MAXIMUM_ALLOWED);
  if (hw) {
    SetProcessWindowStation(hw);
    HDESK hd = OpenDesktopA("Default", 0, FALSE, MAXIMUM_ALLOWED);
    if (hd) SetThreadDesktop(hd);
  }
  HWND hOverlay = FindWindowW(L"CS2_Ostin_ESP", NULL);
  HWND hCS2 = FindWindowA("SDL_app", "Counter-Strike 2");

  RECT rO{}, rCS2{};
  if (hOverlay) GetWindowRect(hOverlay, &rO);
  if (hCS2) GetWindowRect(hCS2, &rCS2);

  printf("CS2 HWND=%p Rect=(%d, %d, %d, %d) Visible=%d\n",
         hCS2, rCS2.left, rCS2.top, rCS2.right, rCS2.bottom, IsWindowVisible(hCS2));
  printf("Overlay HWND=%p Rect=(%d, %d, %d, %d) Visible=%d ExStyle=0x%X\n",
         hOverlay, rO.left, rO.top, rO.right, rO.bottom, IsWindowVisible(hOverlay), (unsigned int)GetWindowLongA(hOverlay, GWL_EXSTYLE));
  return 0;
}
