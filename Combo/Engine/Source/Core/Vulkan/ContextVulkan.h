#ifdef COMBO_VULKAN
#pragma once
#include <Core/Context.h>
#include <optional>
#include "SwapChainVulkan.h"
#include "RenderCommandBufferVulkan.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;
	bool isComplete() {
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

class ContextVulkan : public Context {
private:
	SwapChainVulkan SwapChain;
	RenderCommandBufferVulkan RenderCommandBuffer;
public:
	bool ValidationLayers = false;

	VkInstance Instance;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;

	int FramesInFlight = 2;

	static ContextVulkan* GetVulkanContext();
	SwapChainVulkan* GetSwapChain();
	RenderCommandBufferVulkan* GetRenderCmdBuffer();

	void CreateInstance();
	void CreatePhysicalDevice();
	void CreateDevice();
	void CreateRenderPass();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	bool IsDeviceSuitable(VkPhysicalDevice device);

	std::vector<const char*> GetRequiredExtensions();

	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	virtual void Init();
	virtual void BeginDraw();
	virtual void EndDraw();
	virtual void Draw(Pipeline pipeline);
	virtual void SetClearColor(glm::vec3 color);
	virtual std::string GetApiName();
};
#endif