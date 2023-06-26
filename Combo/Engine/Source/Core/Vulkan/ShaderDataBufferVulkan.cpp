#include "pch.h"
#ifdef COMBO_VULKAN
#include <Core/ShaderDataBuffer.h>
#include "ShaderDataBufferVulkan.h"
#include <Core/Core.h>
#include "ContextVulkan.h"

void ShaderDataBufferVulkan::Init(size_t Size)
{
	this->Size = Size;
}

void ShaderDataBufferVulkan::Update(const void* Data)
{
	
}

std::string ShaderDataBufferVulkan::GetApiName()
{
	return "Vulkan";
}


#endif