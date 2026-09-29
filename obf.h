#pragma once
#include <cstring>
#include <cwchar>

#define OBF_DEC(dst, str) strcpy_s(dst, sizeof(dst), str)
#define OBF_WDEC(dst, wstr) wcscpy_s(dst, sizeof(dst)/sizeof(wchar_t), wstr)
