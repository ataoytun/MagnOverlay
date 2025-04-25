#include "renderer.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Renderer::Renderer() : m_pd3dDevice(NULL), m_pd3dDeviceContext(NULL), m_pSwapChain(NULL), m_mainRenderTargetView(NULL), m_hwnd(NULL), m_initialized(false) { }

Renderer::~Renderer() {
    Cleanup();
}

bool Renderer::Initialize(HWND hwnd) {
    m_hwnd = hwnd;
   
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
        
    if (FAILED(hr)) {
        char errorMsg[256];
        sprintf_s(errorMsg, "D3D11CreateDeviceAndSwapChain failed with HRESULT: 0x%08X", hr);
        MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR);
        return false;
    }

    CreateRenderTarget();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplWin32_Init(hwnd)) {
        MessageBoxA(NULL, "Failed to initialize ImGui Win32 implementation!", "Error", MB_ICONERROR);
        return false;
    }
    
    if (!ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext)) {
        MessageBoxA(NULL, "Failed to initialize ImGui DirectX 11 implementation!", "Error", MB_ICONERROR);
        ImGui_ImplWin32_Shutdown();
        return false;
    }
    
    m_initialized = true;
    return true;
}

void Renderer::BeginFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, clearColor);
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground;
    ImGui::Begin("Overlay", nullptr, window_flags);
}

void Renderer::RenderCircle(const MovingCircle& circle) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(ImVec2(circle.GetX(), circle.GetY()), circle.GetRadius(), IM_COL32(255, 0, 0, 255));
}

void Renderer::EndFrame() {
    ImGui::End();
    ImGui::Render();
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_pSwapChain->Present(1, 0);
}

void Renderer::Cleanup() {
    if (!m_initialized)
        return;
        
    CleanupRenderTarget();
    
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
    if (m_pd3dDeviceContext) { m_pd3dDeviceContext->Release(); m_pd3dDeviceContext = NULL; }
    if (m_pd3dDevice) { m_pd3dDevice->Release(); m_pd3dDevice = NULL; }
    
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    m_initialized = false;
}

void Renderer::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
    pBackBuffer->Release();
}

void Renderer::CleanupRenderTarget() {
    if (m_mainRenderTargetView) { m_mainRenderTargetView->Release(); m_mainRenderTargetView = NULL; }
} 