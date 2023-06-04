#pragma once

#include <Math/Color.h>
#include "../GPURenderData.h"
#include "GPUFramebufferDX11.h"
class GPUFramebuffer;

class GPURenderPass {
public:
	Color32 Color = Color32(0,0,0,1);
	std::vector<GPURenderData> RenderDataList;
	std::shared_ptr<GPUFramebuffer> ActiveFramebuffer;
	void Render(bool RenderImGui = true, std::shared_ptr<GPUFramebuffer> framebuffer = nullptr);
	void SubmitToScreen(bool vSync);
};