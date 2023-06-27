#include "pch.h"
#ifdef COMBO_VULKAN
#include "GFramebufferVulkan.h"
#include "ContextVulkan.h"
#include "../Core.h"


void GFramebufferVulkan::Bind(bool depth)
{

}

void GFramebufferVulkan::Unbind()
{

}

void GFramebufferVulkan::Init()
{
	ImGuiImage = ImGui_ImplVulkan_AddTexture(Sampler, ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}
void* GFramebufferVulkan::GetImage()
{
	return ImageView;
}
void* GFramebufferVulkan::GetImGuiImage()
{
	return ImGuiImage;
}
std::string GFramebufferVulkan::GetApiName()
{
	return "Vulkan";
}
#endif