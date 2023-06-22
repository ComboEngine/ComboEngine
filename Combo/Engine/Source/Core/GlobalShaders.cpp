#include "pch.h"
#include "GlobalShaders.h"

Shader* GlobalShaders::Render3D;
Shader* GlobalShaders::PostFX;

void GlobalShaders::Init()
{
	Shader::Create(&GlobalShaders::Render3D, "./Content/GBuffer.hlsl", "./Content/GBuffer.hlsl");
	Shader::Create(&GlobalShaders::PostFX, "./Content/PostFX.hlsl", "./Content/PostFX.hlsl");
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