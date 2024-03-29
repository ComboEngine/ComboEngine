#include "pch.h"
#ifdef COMBO_VULKAN
#define VMA_IMPLEMENTATION
#include "SwapChainVulkan.h"
#include "../Core.h"
#include "ContextVulkan.h"

uint32_t SwapChainVulkan::GetImageIndex()
{
    return ImageIndex;
}

uint32_t SwapChainVulkan::GetCurrentFrame()
{
    return CurrentFrame;
}

void SwapChainVulkan::InitSurface()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = (HWND)Core::s_Window->GetPlainWindow();
    createInfo.hinstance = GetModuleHandle(nullptr);

    CB_CHECKHR(vkCreateWin32SurfaceKHR(context->Instance, &createInfo, nullptr, &Surface));
}



void SwapChainVulkan::InitDepthBuffer()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();

    VmaAllocator allocator;
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = context->PhysicalDevice;
    allocatorInfo.device = context->Device;
    allocatorInfo.instance = context->Instance;
    vmaCreateAllocator(&allocatorInfo, &allocator);
    
    VkExtent3D depthImageExtent = {
           SwapChainExtent.width,
           SwapChainExtent.height,
           1
    };

    DepthFormat = VK_FORMAT_D32_SFLOAT;

    VkImageCreateInfo info = { };
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = DepthFormat;
    info.extent = depthImageExtent;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vmaCreateImage(allocator, &info, &dimg_allocinfo, &DepthImage, &DepthImageAllocation, nullptr);

    VkImageViewCreateInfo dview_info{};
    dview_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    dview_info.pNext = nullptr;

    dview_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    dview_info.image = DepthImage;
    dview_info.format = DepthFormat;
    dview_info.subresourceRange.baseMipLevel = 0;
    dview_info.subresourceRange.levelCount = 1;
    dview_info.subresourceRange.baseArrayLayer = 0;
    dview_info.subresourceRange.layerCount = 1;
    dview_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    CB_CHECKHR(vkCreateImageView(context->Device, &dview_info, nullptr, &DepthImageView));
}


VkSurfaceFormatKHR SwapChainVulkan::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    VkSurfaceFormatKHR surfaceFormat{};
    surfaceFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
    surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    return surfaceFormat;
}

void SwapChainVulkan::InitFramebuffers()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    SwapChainFramebuffers.resize(SwapChainImageViews.size());

    for (size_t i = 0; i < SwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            SwapChainImageViews[i],
            DepthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = RenderPass;
        framebufferInfo.attachmentCount = 2;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = SwapChainExtent.width;
        framebufferInfo.height = SwapChainExtent.height;
        framebufferInfo.layers = 1;

        CB_CHECKHR(vkCreateFramebuffer(context->Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]));
    }
}

void SwapChainVulkan::Init()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(context->PhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = Surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = context->FindQueueFamilies(context->PhysicalDevice);
    uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

    if (indices.GraphicsFamily != indices.PresentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    CB_CHECKHR(vkCreateSwapchainKHR(context->Device, &createInfo, nullptr, &SwapChain));

    std::vector<VkImage> swapChainImages;
    vkGetSwapchainImagesKHR(context->Device, SwapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(context->Device, SwapChain, &imageCount, swapChainImages.data());

    SwapChainImageFormat = surfaceFormat.format;
    SwapChainExtent = extent;

    SwapChainImageViews.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfoImage{};
        createInfoImage.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfoImage.image = swapChainImages[i];
        createInfoImage.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfoImage.format = SwapChainImageFormat;
        createInfoImage.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfoImage.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfoImage.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfoImage.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfoImage.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfoImage.subresourceRange.baseMipLevel = 0;
        createInfoImage.subresourceRange.levelCount = 1;
        createInfoImage.subresourceRange.baseArrayLayer = 0;
        createInfoImage.subresourceRange.layerCount = 1;

        CB_CHECKHR(vkCreateImageView(context->Device, &createInfoImage, nullptr, &SwapChainImageViews[i]));
    }
}

void SwapChainVulkan::Begin()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    if (!context->StartedFrame) {
        CurrentFrame = (CurrentFrame + 1) % context->FramesInFlight;
        vkWaitForFences(context->Device, 1, &context->GetRenderCmdBuffer()->InFlightFence[CurrentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(context->Device, 1, &context->GetRenderCmdBuffer()->InFlightFence[CurrentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context->Device, SwapChain, UINT64_MAX, context->GetRenderCmdBuffer()->ImageAvailableSemaphore[CurrentFrame], VK_NULL_HANDLE, &imageIndex);
        this->ImageIndex = imageIndex;
    }
}

void SwapChainVulkan::End()
{
}

VkPresentModeKHR SwapChainVulkan::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainVulkan::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(Core::s_Window->GetWidth()),
            static_cast<uint32_t>(Core::s_Window->GetHeight())
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

SwapChainSupportDetails SwapChainVulkan::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}
#endif