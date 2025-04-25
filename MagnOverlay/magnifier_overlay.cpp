#include "magnifier_overlay.h"
#include "utils.h"

MagnifierOverlay::MagnifierOverlay() : m_magnifierWindow(NULL), m_transparencyColor(RGB(0, 0, 0)) { }

MagnifierOverlay::~MagnifierOverlay() { }

bool MagnifierOverlay::Initialize() {
    m_magnifierWindow = FindMagnifierWindow();
    
    if (!m_magnifierWindow) {
        if (!LaunchMagnifier()) {
            return false;
        }
        
        Sleep(2000);
        m_magnifierWindow = FindMagnifierWindow();
        
        if (!m_magnifierWindow) {
            MessageBoxA(NULL, "Magnifier window not found after launching!", "Error", MB_ICONERROR);
            return false;
        }
    }
    
    LONG_PTR originalStyle = GetWindowLongPtr(m_magnifierWindow, GWL_STYLE);
    LONG_PTR originalExStyle = GetWindowLongPtr(m_magnifierWindow, GWL_EXSTYLE);
    SetWindowLongPtr(m_magnifierWindow, GWL_STYLE, originalStyle | WS_VISIBLE);
    SetWindowLongPtr(m_magnifierWindow, GWL_EXSTYLE, (originalExStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT) & ~WS_EX_NOREDIRECTIONBITMAP);
    SetLayeredWindowAttributes(m_magnifierWindow, m_transparencyColor, 0, LWA_COLORKEY);
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(m_magnifierWindow, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
    DrawDebugCircle();
    
    return true;
}

HWND MagnifierOverlay::FindMagnifierWindow() {
    const char* magnifierClasses[] = { "Magnifier", "MagUIClass", "Screen Magnifier Fullscreen Window" };
    return Utils::FindWindowByClasses(magnifierClasses, 3);
}

bool MagnifierOverlay::LaunchMagnifier() {
    SHELLEXECUTEINFOA shellInfo = { 0 };
    shellInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellInfo.lpVerb = "open";
    shellInfo.lpFile = "C:\\Windows\\System32\\Magnify.exe";
    shellInfo.nShow = SW_HIDE;

    if (!ShellExecuteExA(&shellInfo)) {
        MessageBoxA(NULL, "Failed to launch Magnifier!", "Error", MB_ICONERROR);
        return false;
    }

    Sleep(500);
    return true;
}

void MagnifierOverlay::DrawDebugCircle() {
    HDC hdc = GetDC(m_magnifierWindow);
    RECT rect;
    GetClientRect(m_magnifierWindow, &rect);
    
    HBRUSH transparentBrush = CreateSolidBrush(m_transparencyColor);
    FillRect(hdc, &rect, transparentBrush);
    DeleteObject(transparentBrush);
    
    int centerX = rect.right / 2;
    int centerY = rect.bottom / 2;
    int radius = 30;
    
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HPEN redPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    
    SelectObject(hdc, redBrush);
    SelectObject(hdc, redPen);
    
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    
    DeleteObject(redBrush);
    DeleteObject(redPen);
    ReleaseDC(m_magnifierWindow, hdc);
} 