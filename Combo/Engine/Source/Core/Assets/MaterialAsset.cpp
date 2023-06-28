#include "pch.h"
#include "MaterialAsset.h"
#include <Core/Material.h>
#include <Core/GlobalShaders.h>
#include <Core/Core.h>

void MaterialAsset::ImportFromOriginal(std::string BinaryPath)
{
}

void MaterialAsset::CreateEmpty()
{
	this->Handle = new Material();
	this->Handle->Shader = GlobalShaders::GetShader(Render3D);
	this->Handle->Diffuse.Color = glm::vec4(1, 1, 1, 1);
	this->Handle->Diffuse.UseTexture = false;
	this->Handle->Diffuse.ColorTexture = nullptr;
	this->Save();
}

void ReadMaterialProperty(MaterialColor* color, nlohmann::json j) {
	std::cout << j["Color"]["R"];
	color->Color.x = j["Color"]["R"];
	color->Color.y = j["Color"]["G"];
	color->Color.z = j["Color"]["B"];
	color->Color.w = j["Color"]["A"];

	std::string uuid = j["Texture"];
	color->UseTexture = j["UseTexture"];
	if (uuid != "") {
		color->ColorTexture = Core::s_Project.Assets[uuid];
	}
	else {
		color->ColorTexture = nullptr;
	}
}

nlohmann::json SaveMaterialProperty(MaterialColor* color) {
	nlohmann::json j;

	nlohmann::json diffuseVec;
	diffuseVec["R"] = color->Color.x;
	diffuseVec["G"] = color->Color.y;
	diffuseVec["B"] = color->Color.z;
	diffuseVec["A"] = color->Color.w;

	j["Color"] = diffuseVec;
	std::string uuid = color->ColorTexture == nullptr ? "" : color->ColorTexture->UUID;
	j["UseTexture"] = color->UseTexture;
	j["Texture"] = uuid;

	return j;
}

void MaterialAsset::ImportFromEngineType()
{
	std::ifstream t(OSPath.c_str());
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	nlohmann::json j = nlohmann::json::parse(str);

	this->Name = j["Name"];
	this->UUID = j["UUID"];



	Material::Create(&Handle);
	Handle->Shader = GlobalShaders::GetShader(GlobalShader::Render3D);
	ReadMaterialProperty(&Handle->Diffuse, j["Diffuse"]);
}

std::string MaterialAsset::GetType()
{
	return "Material";
}

std::any MaterialAsset::GetHandle()
{
	return this->Handle;
}

void MaterialAsset::Save()
{
	nlohmann::json j;
	j["Name"] = this->Name;
	j["UUID"] = this->UUID;

	j["Diffuse"] = SaveMaterialProperty(&this->Handle->Diffuse);

	std::ofstream file(OSPath);
	file << j.dump(4);
	file.close();
}
