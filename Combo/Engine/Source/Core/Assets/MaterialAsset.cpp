#include "pch.h"
#include "MaterialAsset.h"
#include <nlohmann.h>
#include <fstream>
#include "../GlobalShaders.h"

void MaterialAsset::ReadFromFile(std::string path)
{
	std::ifstream t(path.c_str());
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	nlohmann::json j = nlohmann::json::parse(str);

	this->Name = j["Name"];
	this->uuid = j["UUID"];

	glm::vec4 diffuse = glm::vec4(j["Diffuse"]["R"], j["Diffuse"]["G"], j["Diffuse"]["B"], j["Diffuse"]["A"]);

	Material::Create(&Handle);
	Handle->Shader = GlobalShaders::GetShader(GlobalShader::Render3D);
	Handle->Diffuse = MaterialColor::FromColor(diffuse);
}

void MaterialAsset::ImportToFile(std::string filePath, std::string assetPath, std::any ImportSettings)
{
}

std::any MaterialAsset::GetHandle()
{
	return Handle;
}

std::string MaterialAsset::GetName()
{
	return Name;
}

std::string MaterialAsset::GetType()
{
	return "Material";
}
