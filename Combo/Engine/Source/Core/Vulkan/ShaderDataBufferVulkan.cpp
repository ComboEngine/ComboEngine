#include "pch.h"
#ifdef COMBO_VULKAN
#include <Core/ShaderDataBuffer.h>
#include "ShaderDataBufferVulkan.h"
#include <Core/Core.h>
#include "ContextVulkan.h"
#include "../GlobalShaders.h"
#include "ShaderVulkan.h"

void ShaderDataBufferVulkan::Init(size_t Size)
{
    ContextVulkan* context = ContextVulkan::GetVulkanContext();
}

void ShaderDataBufferVulkan::Update(const void* Data)
{
    
}

std::string ShaderDataBufferVulkan::GetApiName()
{
	return "Vulkan";
}


#endif