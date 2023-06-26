#include "pch.h"
#include "ShaderDataBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ShaderDataBufferDX11.h>
#endif
#ifdef COMBO_VULKAN
#include <Core/Vulkan/ShaderDataBufferVulkan.h>
#endif

void ShaderDataBuffer::Create(ShaderDataBuffer** shaderDataBuffer, size_t Size)
{
	//Select window api

#ifdef COMBO_DIRECTX11
	*shaderDataBuffer = new ShaderDataBufferDX11();
#endif
#ifdef COMBO_VULKAN
	*shaderDataBuffer = new ShaderDataBufferVulkan();
#endif

	//Call API Window
	ShaderDataBuffer* shaderDataBufferPtr = *shaderDataBuffer;
	shaderDataBufferPtr->Init(Size);

	LOG(std::string("Created Shader Data Buffer (" + std::to_string(Size) + " size)"))
}
