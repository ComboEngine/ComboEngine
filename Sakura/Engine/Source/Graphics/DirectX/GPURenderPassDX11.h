#pragma once

#include <Math/Color.h>
#include "../GPURenderData.h"
#include "GPUFramebufferDX11.h"
class GPUFramebuffer;

class GPURenderPass {
public:
	Color32 Color = Color32(0,0,0,1);
	sakura_array<GPURenderData> RenderDataList;
	void Render(bool RenderImGui = true, sakura_ptr<GPUFramebuffer> framebuffer = nullptr);
	void SubmitToScreen(bool vSync);
};