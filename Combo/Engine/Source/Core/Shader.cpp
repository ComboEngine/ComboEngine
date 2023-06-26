#include "pch.h"
#include "Shader.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ShaderDX11.h>
#endif
#ifdef COMBO_VULKAN
#include <Core/Vulkan/ShaderVulkan.h>
#endif

void Shader::Create(Shader** shader, std::string VertexSource,std::string PixelSource)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*shader = new ShaderDX11();
#endif
#ifdef COMBO_VULKAN
	*shader = new ShaderVulkan();
#endif
	Shader* shaderPtr = *shader;
	shaderPtr->Init(VertexSource,PixelSource);
}
