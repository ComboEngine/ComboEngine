#include "pch.h"
#include "Material.h"
#include "GlobalShaders.h"

void Material::Create(Material** Obj)
{
	*Obj = new Material();

	Material* MaterialPtr = *Obj;
	MaterialPtr->Diffuse = MaterialColor::FromColor(glm::vec4(1, 1, 1, 1));
	MaterialPtr->Shader = GlobalShaders::GetShader(GlobalShader::Render3D);
}
