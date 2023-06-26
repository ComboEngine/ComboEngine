#include "pch.h"
#include "GlobalShaders.h"
#include "Core.h"

Shader* GlobalShaders::Render3D;
Shader* GlobalShaders::PostFX;

std::string GlobalShaders::GetShaderPathByApi(std::string name,bool pixel)
{
	switch (Core::RendererType) {
		case DirectX11:
			return "./Content/" + name + ".hlsl";
		case Vulkan:
			return "./Content/" + name + (pixel ? "Frag" : "Vert") + ".spv";
	}
}

void GlobalShaders::Init()
{
	Shader::Create(&GlobalShaders::Render3D, GetShaderPathByApi("GBuffer",false), GetShaderPathByApi("GBuffer", true));
	Shader::Create(&GlobalShaders::PostFX, GetShaderPathByApi("PostFX", false), GetShaderPathByApi("PostFX", true));
}

Shader* GlobalShaders::GetShader(GlobalShader shader)
{
	switch (shader) {
	case GlobalShader::Render3D:
		return GlobalShaders::Render3D;
	case GlobalShader::PostFX:
		return GlobalShaders::PostFX;
	}
}