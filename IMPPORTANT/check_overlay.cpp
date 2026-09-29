#include <windows.h>
#include <cstdio>

int main() {
  HWND hwnd = FindWindowW(L"CS2_Ostin_ESP", NULL);
  printf("FindWindowW(CS2_Ostin_ESP): 0x%p\n", hwnd);
  if (hwnd) {
    RECT r;
    GetWindowRect(hwnd, &r);
    printf("Overlay Rect: (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
    printf("Visible: %d\n", IsWindowVisible(hwnd));
    LONG ex = GetWindowLongA(hwnd, GWL_EXSTYLE);
    printf("ExStyle: 0x%X\n", ex);
  }
  return 0;
}
