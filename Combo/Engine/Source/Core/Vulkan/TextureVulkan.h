#ifdef COMBO_VULKAN
#pragma once
#include <Core/Texture.h>

class TextureVulkan : public Texture {
public:
	virtual void Init(void* mipData, int Width, int Height);
	virtual void ReadFromFramebuffer(Framebuffer* framebuffer);
	virtual std::string GetApiName();
	virtual void* GetImGuiImage();
};
#endif