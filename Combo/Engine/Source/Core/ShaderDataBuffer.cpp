#include "pch.h"
#include "ShaderDataBuffer.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ShaderDataBufferDX11.h>
#endif

void ShaderDataBuffer::Create(Scope<ShaderDataBuffer>& shaderDataBuffer, size_t Size)
{
	shaderDataBuffer = Scope<ShaderDataBuffer>();
	//Select window api

#ifdef COMBO_DIRECTX11
	shaderDataBuffer.Set(new ShaderDataBufferDX11());
#endif

	//Call API Window
	shaderDataBuffer.Get()->Init(Size);

	LOG(std::string("Created Shader Data Buffer (" + std::to_string(Size) + " size)"))
}
