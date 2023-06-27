#ifdef COMBO_VULKAN
#pragma once
#include <Core/Framebuffer.h>


class GFramebufferVulkan : public Framebuffer {
public:
	VkSampler Sampler;
	VkImageView ImageView;
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