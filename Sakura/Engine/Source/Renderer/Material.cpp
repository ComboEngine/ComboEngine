#include "Material.h"
#include <Engine/Engine.h>

sakura_ptr<Material> Material::Create()
{
	sakura_ptr<Material> material = make_shared<Material>();

	material->shader = GPUShader::Create(material->Compile());

	Engine::Materials.push_back(material);

	return material;
}
