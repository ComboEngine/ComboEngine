#include "pch.h"
#ifdef COMBO_VULKAN
#include "RenderCommandBufferVulkan.h"
#include "ContextVulkan.h"
#include "../Core.h"
#include "../ImGui.h"
#include "GBufferVulkan.h"

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

void RenderCommandBufferVulkan::BeginDefaultRenderPass()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    VkClearValue clearColor = { {{context->ClearColor[0], context->ClearColor[1], context->ClearColor[2], context->ClearColor[3]}} };

    VkRenderingAttachmentInfoKHR color_attachment_info{};
    color_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    color_attachment_info.imageView = context->GetSwapChain()->SwapChainImageViews[context->GetSwapChain()->GetImageIndex()];
    color_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    color_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_info.clearValue = clearColor;

    VkClearValue depthClear;
    depthClear.depthStencil.depth = 1.f;

    VkRenderingAttachmentInfoKHR depth_attachment_info{};
    depth_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    depth_attachment_info.imageView = context->GetSwapChain()->DepthImageView;
    depth_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
    depth_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depth_attachment_info.clearValue = depthClear;

    VkRenderingInfoKHR render_info{};
    render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
    render_info.renderArea.offset = { 0, 0 };
    render_info.renderArea.extent = context->GetSwapChain()->SwapChainExtent;
    render_info.layerCount = 1;
    render_info.colorAttachmentCount = 1;
    render_info.pColorAttachments = &color_attachment_info;
    render_info.pDepthAttachment = &depth_attachment_info;

    context->BeginRenderingKHR(CommandBuffer[CurrentFrame], &render_info);
}

void RenderCommandBufferVulkan::Begin()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    
    if (!context->StartedFrame) {
        vkResetCommandBuffer(CommandBuffer[CurrentFrame], 0);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        CB_CHECKHR(vkBeginCommandBuffer(CommandBuffer[CurrentFrame], &beginInfo));
        context->StartedFrame = true;
    }

    if (context->BoundGBuffer) {
        GBufferVulkan* gbuffer = reinterpret_cast<GBufferVulkan*>(Core::s_GBuffer);
        VkClearValue clearColor = { {{context->ClearColor[0], context->ClearColor[1], context->ClearColor[2], context->ClearColor[3]}} };

        std::array<VkRenderingAttachmentInfoKHR, 3> GBufferAttachments;

        VkRenderingAttachmentInfoKHR position_attachment_info{};
        position_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        position_attachment_info.imageView = gbuffer->PositionAttachment.View;
        position_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        position_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        position_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        position_attachment_info.clearValue = clearColor;
        GBufferAttachments[0] = position_attachment_info;

        VkRenderingAttachmentInfoKHR diffuse_attachment_info{};
        diffuse_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        diffuse_attachment_info.imageView = gbuffer->DiffuseAttachment.View;
        diffuse_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        diffuse_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        diffuse_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        diffuse_attachment_info.clearValue = clearColor;
        GBufferAttachments[1] = diffuse_attachment_info;

        VkRenderingAttachmentInfoKHR normal_attachment_info{};
        normal_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        normal_attachment_info.imageView = gbuffer->NormalAttachment.View;
        normal_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        normal_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normal_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        normal_attachment_info.clearValue = clearColor;
        GBufferAttachments[2] = normal_attachment_info;

        VkClearValue depthClear;
        depthClear.depthStencil.depth = 1.f;

        VkRenderingAttachmentInfoKHR depth_attachment_info{};
        depth_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        depth_attachment_info.imageView = gbuffer->DepthAttachment.View;
        depth_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
        depth_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depth_attachment_info.clearValue = depthClear;

        VkRenderingInfoKHR render_info{};
        render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
        render_info.renderArea.offset = { 0, 0 };
        render_info.renderArea.extent = context->GetSwapChain()->SwapChainExtent;
        render_info.layerCount = 1;
        render_info.colorAttachmentCount = GBufferAttachments.size();
        render_info.pColorAttachments = GBufferAttachments.data();
        render_info.pDepthAttachment = &depth_attachment_info;


        context->BeginRenderingKHR(CommandBuffer[CurrentFrame], &render_info);
    }
    else {
        if (context->BoundFramebuffer != nullptr) {
            VkClearValue clearColor = { {{context->ClearColor[0], context->ClearColor[1], context->ClearColor[2], context->ClearColor[3]}} };

            VkRenderingAttachmentInfoKHR color_attachment_info{};
            color_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
            color_attachment_info.imageView = context->BoundFramebuffer->Color.View;
            color_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
            color_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment_info.clearValue = clearColor;

            VkClearValue depthClear;
            depthClear.depthStencil.depth = 1.f;

            VkRenderingAttachmentInfoKHR depth_attachment_info{};
            depth_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
            depth_attachment_info.imageView = context->BoundFramebuffer->Depth.View;
            depth_attachment_info.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
            depth_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depth_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            depth_attachment_info.clearValue = depthClear;

            VkRenderingInfoKHR render_info{};
            render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
            render_info.renderArea.offset = { 0, 0 };
            render_info.renderArea.extent = context->GetSwapChain()->SwapChainExtent;
            render_info.layerCount = 1;
            render_info.colorAttachmentCount = 1;
            render_info.pColorAttachments = &color_attachment_info;
            render_info.pDepthAttachment = &depth_attachment_info;


            context->BeginRenderingKHR(CommandBuffer[CurrentFrame], &render_info);
        }
        else {
            BeginDefaultRenderPass();
        }
    }
}

void RenderCommandBufferVulkan::End()
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
    uint32_t CurrentFrame = context->GetSwapChain()->GetCurrentFrame();
    context->EndRenderingKHR(CommandBuffer[CurrentFrame]);
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

    vkQueueSubmit(GraphicsQueue, 1, &submitInfo, InFlightFence[CurrentFrame]);

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
    context->StartedFrame = false;
}


#endif