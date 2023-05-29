#include "Material.h"

sakura_ptr<Material> Material::Create()
{
	sakura_ptr<Material> material = make_shared<Material>();

	//material->shader = GPUShader::Create(material->GetPixelShader());

	return material;
}
