#include "pch.h"
#include "ImGui.h"
#include "Core.h"


#ifdef COMBO_DIRECTX11
#include "DirectX11/ContextDX11.h"
#include "DirectX11/WindowDX11.h"
#endif
void ImGuiAdapter::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

#ifdef COMBO_DIRECTX11
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    ImGui_ImplGlfw_InitForOther(reinterpret_cast<WindowDX11*>(Core::s_Window)->glfwWindow, true);
    ImGui_ImplDX11_Init(context->Device, context->Context);
#endif

}

void ImGuiAdapter::StartFrame()
{
#ifdef COMBO_DIRECTX11
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();
}

void ImGuiAdapter::EndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
