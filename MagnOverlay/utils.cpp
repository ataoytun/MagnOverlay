#include "utils.h"

namespace Utils {
    std::vector<DWORD> GetPIDs(const wchar_t* targetProcessName) {
        std::vector<DWORD> pids;
        if (!targetProcessName)
            return pids;
        
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(entry);
        
        if (!Process32FirstW(snap, &entry)) {
            CloseHandle(snap);
            return pids;
        }
        
        do {
            if (wcscmp(entry.szExeFile, targetProcessName) == 0) {
                pids.push_back(entry.th32ProcessID);
            }
        } while (Process32NextW(snap, &entry));
        
        CloseHandle(snap);
        return pids;
    }

    HWND FindWindowByClasses(const char* classNames[], int numClasses) {
        HWND window = NULL;
        
        for (int i = 0; i < numClasses && window == NULL; i++) {
            window = FindWindowA(classNames[i], NULL);
        }
        
        return window;
    }
}