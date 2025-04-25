#include <windows.h>
#include <iostream>
#include <string>
#include "magnifier_overlay.h"
#include "renderer.h"
#include "circle.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MagnifierOverlay overlay;
    if (!overlay.Initialize()) {
        return 1;
    }
    
    HWND overlayWindow = overlay.GetWindow();
    MessageBoxA(NULL, "Magnifier window hijacked successfully.\nYou should see a red circle on screen using GDI.\nClick OK to continue with ImGui rendering.", "Debug", MB_OK);

    Renderer renderer;
    if (!renderer.Initialize(overlayWindow)) {
        MessageBoxA(NULL, "Failed to initialize renderer!", "Error", MB_ICONERROR);
        return 1;
    }

    MovingCircle circle(30.0f);
    MessageBoxA(NULL, "Starting ImGui rendering loop.\nYou should see a moving red circle.\nClose Magnifier to exit.", "Info", MB_OK);

    bool done = false;
    while (!done) {
        if (!IsWindow(overlayWindow)) {
            done = true;
            continue;
        }

        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        circle.Update();

        renderer.BeginFrame();
        renderer.RenderCircle(circle);
        renderer.EndFrame();
        
        static int gdiRefreshCounter = 0;
        if (++gdiRefreshCounter >= 300) {
            gdiRefreshCounter = 0;
            overlay.DrawDebugCircle();
        }
    }
    return 0;
}