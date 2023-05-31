#pragma once

#include <Engine/EngineHeaders.h>
#include <Renderer/Material.h>
#include <Utility/UUID.h>
#include <Utility/JSONUtils.h>


class Asset {
public:
	sakura_string uuid;
	sakura_string name;
};

class MaterialAsset : public Asset{
public:
	sakura_string type() { return "Material"; }
	sakura_ptr<Material> Material = nullptr;
	void Init(sakura_string bytes) {
		nlohmann::json j = nlohmann::json::parse(bytes);
		this->name = j["name"];
		this->uuid = j["uuid"];
		this->Material = Material::Create();
		this->Material->albedo = JSONUtils::ReadColorFromJson(j["albedo"]);
		this->Material->Init();
	}
};

class MeshAsset : public Asset {
public:
	sakura_string type() { return "Mesh"; }
	sakura_ptr<Mesh> Mesh = nullptr;
	void Init(sakura_string bytes) {
		Vertex vertices[] =
		{
			Vertex(-0.5f, -0.5f, 0.5f,1.0f,1.0f,1.0f,1.0f,1.0f),
			Vertex(-0.5f,  0.5f, 0.5f,1.0f,0.0f,1.0f,1.0f,1.0f),
			Vertex(0.5f,  0.5f, 0.5f,0.0f,0.0f,1.0f,1.0f,1.0f),
			Vertex(0.5f, -0.5f, 0.5f,0.0f,1.0f,1.0f,1.0f,1.0f),
		};

		DWORD indices[] = {
		0, 1, 2,
		0, 2, 3,
		};
		this->Mesh = GPU::Instance->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
	}
};