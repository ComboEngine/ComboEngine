#include "pch.h"
#ifdef COMBO_VULKAN
#include "TextureVulkan.h"
#include "ContextVulkan.h"
#include "../Core.h"
#include "FramebufferVulkan.h"

void TextureVulkan::Init(void* mipData, int Width, int Height)
{
	
}

void TextureVulkan::ReadFromFramebuffer(Framebuffer* framebuffer)
{
	
}

std::string TextureVulkan::GetApiName()
{
	return "Vulkan";
}

void* TextureVulkan::GetImGuiImage()
{
	return nullptr;
}

#endif