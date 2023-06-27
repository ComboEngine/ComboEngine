#include "pch.h"
#ifdef COMBO_VULKAN
#include "GBufferVulkan.h"
#include "../Core.h"
#include "ContextVulkan.h"
#include "FramebufferVulkan.h"
#include "GFramebufferVulkan.h"

void GBufferVulkan::ApiInit()
{
	ContextVulkan* context = ContextVulkan::GetVulkanContext();
	Width = Core::s_Window->GetWidth();
	Height = Core::s_Window->GetHeight();

	CreateAttachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,&PositionAttachment);
	CreateAttachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,&NormalAttachment);
	CreateAttachment(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, &DiffuseAttachment);
	CreateAttachment(VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &DepthAttachment);

	VkSampler Sampler;

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = samplerInfo.addressModeU;
	samplerInfo.addressModeW = samplerInfo.addressModeU;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	CB_CHECKHR(vkCreateSampler(context->Device, &samplerInfo, nullptr, &Sampler));

	Position = new GFramebufferVulkan();
	GFramebufferVulkan* position = reinterpret_cast<GFramebufferVulkan*>(Position);
	position->ImageView = PositionAttachment.View;
	position->Sampler = Sampler;
	position->Init();

	Normal = new GFramebufferVulkan();
	GFramebufferVulkan* normal = reinterpret_cast<GFramebufferVulkan*>(Normal);
	normal->ImageView = NormalAttachment.View;
	normal->Sampler = Sampler;
	normal->Init();

	Diffuse = new GFramebufferVulkan();
	GFramebufferVulkan* diffuse = reinterpret_cast<GFramebufferVulkan*>(Diffuse);
	diffuse->ImageView = DiffuseAttachment.View;
	diffuse->Sampler = Sampler;
	diffuse->Init();

}   

void GBufferVulkan::Bind()
{
	ContextVulkan* context = reinterpret_cast<ContextVulkan*>(Core::s_Context);
	context->BoundGBuffer = true;
}

void GBufferVulkan::Unbind()
{
    ContextVulkan* context = reinterpret_cast<ContextVulkan*>(Core::s_Context);
	context->EndRenderingKHR(context->GetRenderCmdBuffer()->CommandBuffer[context->GetSwapChain()->GetCurrentFrame()]);
	context->BoundGBuffer = false;
}

void GBufferVulkan::CreateAttachment(VkFormat format, VkImageUsageFlagBits usage, GFrameBufferAttachment* attachment)
{
	ContextVulkan* context = ContextVulkan::GetVulkanContext();
	VkImageAspectFlags aspectMask = 0;
	VkImageLayout imageLayout;

	attachment->Format = format;

	if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
	{
		aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}
	if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (format >= VK_FORMAT_D16_UNORM_S8_UINT)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	assert(aspectMask > 0);

	VkImageCreateInfo image{};
	image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image.imageType = VK_IMAGE_TYPE_2D;
	image.format = format;
	image.extent.width = Width;
	image.extent.height = Height;
	image.extent.depth = 1;
	image.mipLevels = 1;
	image.arrayLayers = 1;
	image.samples = VK_SAMPLE_COUNT_1_BIT;
	image.tiling = VK_IMAGE_TILING_OPTIMAL;
	image.usage = usage | VK_IMAGE_USAGE_SAMPLED_BIT;

	VkMemoryAllocateInfo memAlloc{};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	VkMemoryRequirements memReqs;

	CB_CHECKHR(vkCreateImage(context->Device, &image, nullptr, &attachment->Image));
	vkGetImageMemoryRequirements(context->Device, attachment->Image, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = context->FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	CB_CHECKHR(vkAllocateMemory(context->Device, &memAlloc, nullptr, &attachment->Mem));
	CB_CHECKHR(vkBindImageMemory(context->Device, attachment->Image, attachment->Mem, 0));

	VkImageViewCreateInfo imageView{};
	imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageView.format = format;
	imageView.subresourceRange = {};
	imageView.subresourceRange.aspectMask = aspectMask;
	imageView.subresourceRange.baseMipLevel = 0;
	imageView.subresourceRange.levelCount = 1;
	imageView.subresourceRange.baseArrayLayer = 0;
	imageView.subresourceRange.layerCount = 1;
	imageView.image = attachment->Image;
	CB_CHECKHR(vkCreateImageView(context->Device, &imageView, nullptr, &attachment->View));
}

#endif