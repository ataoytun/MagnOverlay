#pragma once
#include <windows.h>
#include <string>
#include <shellapi.h>

class MagnifierOverlay {
public:
    MagnifierOverlay();
    ~MagnifierOverlay();

    bool Initialize();
    HWND GetWindow() const { return m_magnifierWindow; }
    void DrawDebugCircle();

private:
    HWND FindMagnifierWindow();
    bool LaunchMagnifier();
    
    HWND m_magnifierWindow;
    COLORREF m_transparencyColor;
}; 