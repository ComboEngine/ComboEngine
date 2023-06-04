#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <Graphics/GPU.h>


class GPUImGuiDX11 {
public:
	static void Create(GPU* instance);
	static void Render();
};