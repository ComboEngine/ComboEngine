#include "pch.h"
#include "MeshAsset.h"
#include <fstream>
#include <Utility/JSON.h>
#include <Renderer/Vertex.h>
#include <Graphics/GPU.h>

void MeshSerializer::Save(std::shared_ptr<Mesh> mesh,std::string path)
{
	std::ofstream file;
	file.open(path, std::ios::binary | std::ios::out);

	uint32_t size = mesh->Submeshes.size();
	file.write((const char*)&size, sizeof(uint32_t));
	for (std::shared_ptr<Submesh> submesh : mesh->Submeshes) {
		nlohmann::json details;
		details["Name"] = submesh->Name;
		details["VerticesSize"] = submesh->Vertices.size();
		details["IndicesSize"] = submesh->Indices.size();
		std::string jsonString = details.dump();
		uint32_t jsonSize = jsonString.size();
		file.write((const char*)&jsonSize, sizeof(uint32_t));
		file.write(jsonString.data(), jsonSize);
		for (Vertex vertex : submesh->Vertices) {
			file.write((const char*)&vertex, sizeof(Vertex));
		}
		for (DWORD index : submesh->Indices) {
			file.write((const char*)&index, sizeof(DWORD));
		}
	}


	file.close();
}

std::shared_ptr<Mesh> MeshSerializer::Read(std::string path)
{
	std::ifstream infile;
	infile.open(path, std::ios::binary);

	infile.seekg(0);

	uint32_t size;
	infile.read((char*)&size, sizeof(uint32_t));

	std::vector<std::shared_ptr<Submesh>> submeshesList;

	for (int i = 0; i < size; i++) {
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

		std::string jsonStr;
		uint32_t jsonSize;
		infile.read((char*)&jsonSize, sizeof(uint32_t));
		jsonStr.resize(jsonSize);
		infile.read(jsonStr.data(), jsonSize);
		nlohmann::json json = nlohmann::json::parse(jsonStr);

		for (int j = 0; j < json["VerticesSize"]; j++) {
			Vertex v;
			infile.read((char*)&v, sizeof(Vertex));
			vertices.push_back(v);
		}

		for (int j = 0; j < json["IndicesSize"]; j++) {
			DWORD d;
			infile.read((char*)&d, sizeof(DWORD));
			indices.push_back(d);
		}
		std::shared_ptr<Submesh> submesh = GPU::Instance->CreateSubmesh(vertices, indices);
		submesh->Name = json["Name"];
		submeshesList.push_back(submesh);
	}

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->Submeshes = submeshesList;

	return mesh;
}
