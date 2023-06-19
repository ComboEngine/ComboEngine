#include "pch.h"
#include "GlobalShaders.h"

Scope<Shader> GlobalShaders::Render3D;

void GlobalShaders::Init()
{
	Shader::Create(GlobalShaders::Render3D, "./Content/shader.hlsl", "./Content/shader.hlsl");
}

Scope<Shader> GlobalShaders::GetShader(GlobalShader shader)
{
	switch (shader) {
	case GlobalShader::Render3D:
		return GlobalShaders::Render3D;
	}
}