#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>

namespace Utils {
    std::vector<DWORD> GetPIDs(const wchar_t* targetProcessName);
    HWND FindWindowByClasses(const char* classNames[], int numClasses);
}