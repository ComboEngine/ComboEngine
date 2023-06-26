#include "pch.h"
#ifdef COMBO_VULKAN
#include "RenderCommandBufferVulkan.h"
#include "ContextVulkan.h"

VkCommandBuffer RenderCommandBufferVulkan::GetCommandBuffer()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    return CommandBuffer[CurrentFrame];
}

void RenderCommandBufferVulkan::Init()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    QueueFamilyIndices queueFamilyIndices = context->FindQueueFamilies(context->PhysicalDevice);

    CommandBuffer.resize(context->FramesInFlight);
    ImageAvailableSemaphore.resize(context->FramesInFlight);
    RenderFinishedSemaphore.resize(context->FramesInFlight);
    InFlightFence.resize(context->FramesInFlight);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

    CB_CHECKHR(vkCreateCommandPool(context->Device, &poolInfo, nullptr, &CommandPool));

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = CommandBuffer.size();

    CB_CHECKHR(vkAllocateCommandBuffers(context->Device, &allocInfo, CommandBuffer.data()));

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < context->FramesInFlight; i++) {
        if (vkCreateSemaphore(context->Device, &semaphoreInfo, nullptr, &ImageAvailableSemaphore[i]) != VK_SUCCESS ||
            vkCreateSemaphore(context->Device, &semaphoreInfo, nullptr, &RenderFinishedSemaphore[i]) != VK_SUCCESS ||
            vkCreateFence(context->Device, &fenceInfo, nullptr, &InFlightFence[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}

void RenderCommandBufferVulkan::Begin()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    vkResetCommandBuffer(CommandBuffer[CurrentFrame], 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    CB_CHECKHR(vkBeginCommandBuffer(CommandBuffer[CurrentFrame], &beginInfo));

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = context->GetSwapChain()->RenderPass;
    renderPassInfo.framebuffer = context->GetSwapChain()->SwapChainFramebuffers[context->GetSwapChain()->GetImageIndex()];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = context->GetSwapChain()->SwapChainExtent;

    VkClearValue clearColor = { {{context->ClearColor[0], context->ClearColor[1], context->ClearColor[2], context->ClearColor[3]}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(CommandBuffer[CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderCommandBufferVulkan::End()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    vkCmdEndRenderPass(CommandBuffer[CurrentFrame]);
    CB_CHECKHR(vkEndCommandBuffer(CommandBuffer[CurrentFrame]));

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { ImageAvailableSemaphore[CurrentFrame]};
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &CommandBuffer[CurrentFrame];

    VkSemaphore signalSemaphores[] = { RenderFinishedSemaphore[CurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    CB_CHECKHR(vkQueueSubmit(GraphicsQueue, 1, &submitInfo, InFlightFence[CurrentFrame]));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { context->GetSwapChain()->SwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    uint32_t index = context->GetSwapChain()->GetImageIndex();
    presentInfo.pImageIndices = &index;

    vkQueuePresentKHR(PresentQueue, &presentInfo);
}


#endif