#include "pch.h"
#include "Material.h"
#include "GlobalShaders.h"

void Material::Create(Scope<Material>& Obj)
{
	Scope<Material>::Create(Obj);

	Obj.Get()->Diffuse = MaterialColor::FromColor(glm::vec4(1, 1, 1, 1));
	Obj.Get()->Shader = GlobalShaders::GetShader(GlobalShader::Render3D);
}
