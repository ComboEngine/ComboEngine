#include "pch.h"
#ifdef COMBO_VULKAN
#include "ContextVulkan.h"
#include <Core/Core.h>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include "ShaderVulkan.h"
#include "VertexBufferVulkan.h"
#include "IndexBufferVulkan.h"
#include "ShaderDataBufferVulkan.h"
#include "../Mesh.h"
/*
#include "WindowVulkan.h"
#include "ShaderVulkan.h"
#include "VertexBufferVulkan.h"
#include "IndexBufferVulkan.h"
#include "ShaderDataBufferVulkan.h"
#include "TextureVulkan.h"
#include "FramebufferVulkan.h"*/


ContextVulkan* ContextVulkan::GetVulkanContext()
{
    return reinterpret_cast<ContextVulkan*>(Core::s_Context);
}

SwapChainVulkan* ContextVulkan::GetSwapChain()
{
    return &SwapChain;
}

RenderCommandBufferVulkan* ContextVulkan::GetRenderCmdBuffer()
{
    return &RenderCommandBuffer;
}

void ContextVulkan::SubmitBarrier(std::function<void(VkCommandBuffer cmd)>&& function)
{
    VkCommandBuffer cmd = UploadContext.CommandBuffer;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    CB_CHECKHR(vkBeginCommandBuffer(cmd, &beginInfo));

    function(cmd);

    CB_CHECKHR(vkEndCommandBuffer(cmd));

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;

    CB_CHECKHR(vkQueueSubmit(RenderCommandBuffer.GraphicsQueue, 1, &submitInfo, UploadContext.UploadFence));

    vkWaitForFences(Device, 1, &UploadContext.UploadFence, true, 9999999999);
    vkResetFences(Device, 1, &UploadContext.UploadFence);

    vkResetCommandPool(Device, UploadContext.CommandPool, 0);
}

void ContextVulkan::SubmitBarrierAtEnd(std::function<void(VkCommandBuffer cmd)> function)
{
    EndBarriers.push_back(function);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    //LOG("Validation layer: " + std::string(pCallbackData->pMessage))

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void ContextVulkan::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Core::s_Window->Specification.Title.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "ComboEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkDebugUtilsMessengerCreateInfoEXT createInfoMessenger{};
    createInfoMessenger.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfoMessenger.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfoMessenger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfoMessenger.pfnUserCallback = debugCallback;
    createInfoMessenger.pUserData = nullptr; // Optional

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> Extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = Extensions.size();
    createInfo.ppEnabledExtensionNames = Extensions.data();

    if (ValidationLayers) {
        createInfo.enabledLayerCount = 1;
        const char* const ext[] = {
            "VK_LAYER_KHRONOS_validation"
        };
        createInfo.ppEnabledLayerNames = ext;

        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&createInfoMessenger;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    CB_CHECKHR(vkCreateInstance(&createInfo, nullptr, &this->Instance));

    if (ValidationLayers) {
        CB_CHECKHR(CreateDebugUtilsMessengerEXT(Instance, &createInfoMessenger, nullptr, &this->DebugMessenger));
    }
}

void ContextVulkan::CreatePhysicalDevice()
{
    uint32_t deviceCount = 0;
    CB_CHECKHR(vkEnumeratePhysicalDevices(this->Instance, &deviceCount, nullptr));
    if (deviceCount == 0) {
        CB_ASSERT("No GPUs supporting vulkan were found")
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    CB_CHECKHR(vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data()));

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            PhysicalDevice = device;
            break;
        }
    }

    if (PhysicalDevice == VK_NULL_HANDLE) {
        CB_ASSERT("Failed to find a suitable GPU!")
    }
}




void ContextVulkan::CreateDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceVulkan13Features features1_2{};
    features1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    features1_2.dynamicRendering = VK_TRUE;

    VkPhysicalDeviceFeatures2 features2{};
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &features1_2;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pNext = &features2;
    createInfo.enabledExtensionCount = 0;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.enabledLayerCount = 0;
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_dynamic_rendering",
        "VK_KHR_depth_stencil_resolve",
        "VK_KHR_create_renderpass2"
    };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    CB_CHECKHR(vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &Device));

    vkGetDeviceQueue(Device, indices.GraphicsFamily.value(), 0, &RenderCommandBuffer.GraphicsQueue);
    vkGetDeviceQueue(Device, indices.PresentFamily.value(), 0, &RenderCommandBuffer.PresentQueue);
}

void ContextVulkan::CreateUploadContext()
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(PhysicalDevice);

    CB_CHECKHR(vkCreateFence(Device, &fenceInfo, nullptr, &UploadContext.UploadFence));

    VkCommandPoolCreateInfo uploadCommandPoolInfo = {};
    uploadCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    uploadCommandPoolInfo.pNext = nullptr;
    uploadCommandPoolInfo.queueFamilyIndex = indices.GraphicsFamily.value();
    uploadCommandPoolInfo.flags = 0;

    CB_CHECKHR(vkCreateCommandPool(Device, &uploadCommandPoolInfo, nullptr, &UploadContext.CommandPool));


    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = UploadContext.CommandPool;
    info.commandBufferCount = 1;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmd;
    CB_CHECKHR(vkAllocateCommandBuffers(Device, &info, &UploadContext.CommandBuffer));
}



void ContextVulkan::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = SwapChain.SwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment = {};
    depth_attachment.flags = 0;
    depth_attachment.format = SwapChain.DepthFormat;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency depth_dependency = {};
    depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    depth_dependency.dstSubpass = 0;
    depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.srcAccessMask = 0;
    depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


    std::array< VkAttachmentDescription,2> attachments = { colorAttachment,depth_attachment };
    std::array<VkSubpassDependency,2> dependencies = { dependency, depth_dependency };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 2;
    renderPassInfo.pDependencies = dependencies.data();


    CB_CHECKHR(vkCreateRenderPass(Device, &renderPassInfo, nullptr, &SwapChain.RenderPass));
}



bool ContextVulkan::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    bool swapChainAdequate = false;
    if (requiredExtensions.empty()) {
        SwapChainSupportDetails swapChainSupport = SwapChain.QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && requiredExtensions.empty() && swapChainAdequate;
}

VkResult ContextVulkan::BeginRenderingKHR(VkCommandBuffer cmdBuffer, VkRenderingInfo* renderingInfo)
{
    vkCmdBeginRendering(cmdBuffer, renderingInfo);
    return VkResult();
}

VkResult ContextVulkan::EndRenderingKHR(VkCommandBuffer cmdBuffer)
{
    vkCmdEndRendering(cmdBuffer);
    return VkResult();
}


QueueFamilyIndices ContextVulkan::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.GraphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, SwapChain.Surface, &presentSupport);

        if (presentSupport) {
            indices.PresentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

uint32_t ContextVulkan::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    CB_ASSERT("Cannot find memory type!")
    return 0;
}


std::vector<const char*> ContextVulkan::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
    this->ValidationLayers = true;
#endif

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool ValidationLayersFound = true;
    for (const auto& layer : availableLayers) {
        if (layer.layerName == "VK_LAYER_KHRONOS_validation") {
            ValidationLayersFound = true;
            break;
        }
    }

    this->ValidationLayers = this->ValidationLayers ? ValidationLayersFound : false;

    if (this->ValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void ContextVulkan::Init()
{
    this->CreateInstance();
    LOG("Created VK Instance");
    SwapChain.InitSurface();
    LOG("Created Surface");
    this->CreatePhysicalDevice();
    LOG("Created VK Physical Device");
    this->CreateDevice();
    LOG("Created VK Device");
    SwapChain.Init();
    SwapChain.InitDepthBuffer();
    LOG("Created VK SwapChain");
    this->CreateUploadContext();
    this->CreateRenderPass();
    LOG("Created VK RenderPass");
    SwapChain.InitFramebuffers();
    LOG("Created VK Framebuffers");

    // VkFuncBeginRendering = (PFN_vkCmdBeginRenderingKHR)vkGetInstanceProcAddr(Instance, "vkCmdBeginRenderingKHR");
 //VkFuncEndRendering = (PFN_vkCmdEndRenderingKHR)vkGetInstanceProcAddr(Instance, "vkCmdEndRenderingKHR");

    std::cout << "Finished loading functions" << std::endl;

    this->RenderCommandBuffer.Init();
    LOG("Created Render Command Buffer")


    Core::ExitEvent.Hook([&] {
        vkDestroyInstance(Instance, nullptr);
        vkDestroyDevice(Device, nullptr);
    });
}

void ContextVulkan::BeginDraw()
{
    SwapChain.Begin();
    RenderCommandBuffer.Begin();

}
void ContextVulkan::EndDraw()
{
    for (std::function<void(VkCommandBuffer cmd)> barrier : EndBarriers) {
        barrier(RenderCommandBuffer.CommandBuffer[SwapChain.GetCurrentFrame()]);
    }
    EndBarriers.clear();
    RenderCommandBuffer.End();
}
void ContextVulkan::Draw(Pipeline pipeline)
{
    ShaderVulkan* shader = reinterpret_cast<ShaderVulkan*>(pipeline.Shader);
    vkCmdBindPipeline(RenderCommandBuffer.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GraphicsPipeline);

    VkBuffer vertexBuffers[] = { reinterpret_cast<VertexBufferVulkan*>(pipeline.VertexBuffer)->Buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(RenderCommandBuffer.GetCommandBuffer(), 0, 1, vertexBuffers, offsets);
    if (pipeline.Indexed) {
        vkCmdBindIndexBuffer(RenderCommandBuffer.GetCommandBuffer(), reinterpret_cast<IndexBufferVulkan*>(pipeline.IndexBuffer)->Buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChain.SwapChainExtent.width);
    viewport.height = static_cast<float>(SwapChain.SwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(RenderCommandBuffer.GetCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = SwapChain.SwapChainExtent;
    vkCmdSetScissor(RenderCommandBuffer.GetCommandBuffer(), 0, 1, &scissor);

    if (pipeline.VulkanPushConstant != nullptr) {
        vkCmdPushConstants(RenderCommandBuffer.GetCommandBuffer(), shader->PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(MeshShaderData), pipeline.VulkanPushConstant);
    }
    


    if (pipeline.Indexed) {
        vkCmdDrawIndexed(RenderCommandBuffer.GetCommandBuffer(), reinterpret_cast<IndexBufferVulkan*>(pipeline.IndexBuffer)->Size, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(RenderCommandBuffer.GetCommandBuffer(), reinterpret_cast<VertexBufferVulkan*>(pipeline.VertexBuffer)->Size, 1, 0, 0);
    }
}

void ContextVulkan::SetClearColor(glm::vec3 color)
{
    this->ClearColor[0] = color.x;
    this->ClearColor[1] = color.y;
    this->ClearColor[2] = color.z;
    this->ClearColor[3] = 1;
}

std::string ContextVulkan::GetApiName()
{
	return "Vulkan";
}
#endif