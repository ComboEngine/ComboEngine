#ifdef COMBO_VULKAN
#pragma once
#include "pch.h"

class RenderCommandBufferVulkan {
public:
	std::vector<VkSemaphore> ImageAvailableSemaphore;
	std::vector<VkSemaphore> RenderFinishedSemaphore;
	std::vector<VkFence> InFlightFence;
	VkCommandPool CommandPool;
	std::vector<VkCommandBuffer> CommandBuffer;
	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	VkCommandBuffer GetCommandBuffer();
	void Init();
	void BeginDefaultRenderPass();
	void Begin();
	void End();
};
#endif
