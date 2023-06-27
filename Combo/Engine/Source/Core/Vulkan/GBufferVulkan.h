#ifdef COMBO_VULKAN
#pragma once
#include <Core/GBuffer.h>

struct GFrameBufferAttachment {
	VkImage Image;
	VkDeviceMemory Mem;
	VkImageView View;
	VkFormat Format;
};

class GBufferVulkan : public GBuffer {
public:
	int32_t Width, Height;
	GFrameBufferAttachment PositionAttachment, NormalAttachment, DiffuseAttachment, DepthAttachment;
	VkSampler Sampler;
	virtual void ApiInit();
	virtual void Bind();
	virtual void Unbind();
	void CreateAttachment(
		VkFormat format,
		VkImageUsageFlagBits usage,
		GFrameBufferAttachment* attachment);
};
#endif