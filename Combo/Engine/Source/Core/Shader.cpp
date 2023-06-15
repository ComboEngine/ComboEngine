#include "pch.h"
#include "Shader.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ShaderDX11.h>
#endif

void Shader::Create(Scope<Shader>& shader, std::string VertexSource,std::string PixelSource)
{
	shader = Scope<Shader>();
	//Select window api
#ifdef COMBO_DIRECTX11
	shader.Set(new ShaderDX11());
#endif
	shader.Get()->Init(VertexSource,PixelSource);
}
