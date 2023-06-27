#ifdef COMBO_VULKAN
#pragma once
#include <Core/Context.h>
#include <optional>
#include "SwapChainVulkan.h"
#include "RenderCommandBufferVulkan.h"
#include "FramebufferVulkan.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;
	bool isComplete() {
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

struct UploadContext {
	VkFence UploadFence;
	VkCommandPool CommandPool;
	VkCommandBuffer CommandBuffer;
};

class ContextVulkan : public Context {
private:
	SwapChainVulkan SwapChain;
	RenderCommandBufferVulkan RenderCommandBuffer;
public:
	UploadContext UploadContext;
	bool ValidationLayers = false;
	bool StartedFrame = false;
	FramebufferVulkan* BoundFramebuffer;
	std::vector<std::function<void(VkCommandBuffer cmd)>> EndBarriers;

	VkInstance Instance;
	VkDebugUtilsMessengerEXT DebugMessenger;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;

	PFN_vkCmdBeginRenderingKHR VkFuncBeginRendering = NULL;
	PFN_vkCmdEndRenderingKHR VkFuncEndRendering = NULL;

	int FramesInFlight = 2;

	static ContextVulkan* GetVulkanContext();
	SwapChainVulkan* GetSwapChain();
	RenderCommandBufferVulkan* GetRenderCmdBuffer();
	void SubmitBarrier(std::function<void(VkCommandBuffer cmd)>&& function);
	void SubmitBarrierAtEnd(std::function<void(VkCommandBuffer cmd)> function);

	void CreateInstance();
	void CreatePhysicalDevice();
	void CreateDevice();
	void CreateUploadContext();
	void CreateRenderPass();


	VkResult BeginRenderingKHR(VkCommandBuffer cmdBuffer, VkRenderingInfo* renderingInfo);
	VkResult EndRenderingKHR(VkCommandBuffer cmdBuffer);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	bool IsDeviceSuitable(VkPhysicalDevice device);
	std::vector<const char*> GetRequiredExtensions();

	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	bool BoundGBuffer = false;
	virtual void Init();
	virtual void BeginDraw();
	virtual void EndDraw();
	virtual void Draw(Pipeline pipeline);
	virtual void SetClearColor(glm::vec3 color);
	virtual std::string GetApiName();
};
#endif