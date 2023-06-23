#include "pch.h"
#include "MaterialAsset.h"
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
	this->Handle = new Material();
	nlohmann::json j;
	j["Name"] = this->Name;
	j["UUID"] = this->uuid;

	nlohmann::json diffuseVec;
	diffuseVec["R"] = this->Handle->Diffuse.Color.x;
	diffuseVec["G"] = this->Handle->Diffuse.Color.y;
	diffuseVec["B"] = this->Handle->Diffuse.Color.z;
	diffuseVec["A"] = this->Handle->Diffuse.Color.w;

	j["Diffuse"] = diffuseVec;

	std::ofstream file(filePath);
	file << j.dump(4);
	file.close();
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
