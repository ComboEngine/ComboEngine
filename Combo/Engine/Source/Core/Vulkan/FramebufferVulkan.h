#ifdef COMBO_VULKAN
#pragma once
#include <Core/Framebuffer.h>

struct FrameBufferAttachment {
	VkImage Image;
	VkDeviceMemory Mem;
	VkImageView View;
};

class FramebufferVulkan : public Framebuffer {
public:
	VkFramebuffer FrameBuffer;
	FrameBufferAttachment Color, Depth;
	VkRenderPass RenderPass;
	VkSampler Sampler;
	VkDescriptorImageInfo Descriptor;
	VkDescriptorSet ImGuiImage;

	bool BoundWithDepth = false;
	virtual void Bind(bool depth);
	virtual void Unbind();
	virtual void Init();
	virtual void* GetImage();
	virtual void* GetImGuiImage();
	virtual std::string GetApiName();
};
#endif