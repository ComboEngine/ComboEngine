#ifdef COMBO_VULKAN
#pragma once
#include "pch.h"

#include <vk_mem_alloc.h>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChainVulkan {
private:
	uint32_t ImageIndex;
	uint32_t CurrentFrame;
public:
	VkSwapchainKHR SwapChain;
	std::vector<VkImage> SwapChainImages;
	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;
	VkRenderPass RenderPass;
	std::vector<VkImageView> SwapChainImageViews;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	VkSurfaceKHR Surface;

	VkImage DepthImage;
	VmaAllocation DepthImageAllocation;
	VkFormat DepthFormat;
	VkImageView DepthImageView;

	
	uint32_t GetImageIndex();
	uint32_t GetCurrentFrame();
	void InitSurface();
	void InitDepthBuffer();
	void InitFramebuffers();
	void Init();
	void Begin();
	void End();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
};
#endif 
