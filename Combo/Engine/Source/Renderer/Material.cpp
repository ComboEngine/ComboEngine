#include "pch.h"
#include "Material.h"
#include <Engine/Engine.h>
#include <fstream>

void Material::Init()
{
    this->shader = GPUShader::Create(this->Compile());
}

std::shared_ptr<Material> Material::Create()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	Engine::Materials.push_back(material);

	return material;
}

std::string Material::Compile()
{
	if (this->Base == PBR) {
		std::ifstream t("Shaders/PBR.hlsl");
		return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	}
}