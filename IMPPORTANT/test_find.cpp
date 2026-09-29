#include <windows.h>
#include <cstdio>

int main() {
  HWND h1 = FindWindowW(L"CS2_Ostin_ESP", NULL);
  HWND h2 = FindWindowW(NULL, L"CS2 Ostin ESP Overlay");
  HWND h3 = FindWindowW(L"CS2_Ostin_ESP", L"CS2 Ostin ESP Overlay");
  printf("By Class: 0x%p, By Title: 0x%p, By Both: 0x%p\n", h1, h2, h3);
  return 0;
}
