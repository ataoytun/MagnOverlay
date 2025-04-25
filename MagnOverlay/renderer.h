#pragma once
#include <d3d11.h>
#include <windows.h>
#include "circle.h"

struct ImDrawList;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd);
    void BeginFrame();
    void RenderCircle(const MovingCircle& circle);
    void EndFrame();
    void Cleanup();

private:
    void CreateRenderTarget();
    void CleanupRenderTarget();

    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pd3dDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_mainRenderTargetView;
    HWND m_hwnd;
    bool m_initialized;
}; 