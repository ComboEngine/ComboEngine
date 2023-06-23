#include "pch.h"
#include "MaterialAsset.h"
#include <Core/Material.h>
#include <Core/GlobalShaders.h>

void MaterialAsset::ImportFromOriginal(std::string BinaryPath)
{
}

void MaterialAsset::CreateEmpty()
{
	this->Handle = new Material();
	this->Handle->Shader = GlobalShaders::GetShader(Render3D);
	this->Handle->Diffuse.Color = glm::vec4(1, 1, 1, 1);

	nlohmann::json j;
	j["Name"] = this->Name;
	j["UUID"] = this->UUID;

	nlohmann::json diffuseVec;
	diffuseVec["R"] = this->Handle->Diffuse.Color.x;
	diffuseVec["G"] = this->Handle->Diffuse.Color.y;
	diffuseVec["B"] = this->Handle->Diffuse.Color.z;
	diffuseVec["A"] = this->Handle->Diffuse.Color.w;

	j["Diffuse"] = diffuseVec;

	std::ofstream file(OSPath);
	file << j.dump(4);
	file.close();
}

void MaterialAsset::ImportFromEngineType()
{
	std::ifstream t(OSPath.c_str());
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	nlohmann::json j = nlohmann::json::parse(str);

	this->Name = j["Name"];
	this->UUID = j["UUID"];

	glm::vec4 diffuse = glm::vec4(j["Diffuse"]["R"], j["Diffuse"]["G"], j["Diffuse"]["B"], j["Diffuse"]["A"]);

	Material::Create(&Handle);
	Handle->Shader = GlobalShaders::GetShader(GlobalShader::Render3D);
	Handle->Diffuse = MaterialColor::FromColor(diffuse);
}

std::string MaterialAsset::GetType()
{
	return "Material";
}

std::any MaterialAsset::GetHandle()
{
	return this->Handle;
}
