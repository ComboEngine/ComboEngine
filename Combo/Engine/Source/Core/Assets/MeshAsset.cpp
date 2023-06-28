#include "pch.h"
#include "MeshAsset.h"
#include <Core/Material.h>
#include <Core/GlobalShaders.h>

void MeshAsset::ImportFromOriginal(std::string BinaryPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(BinaryPath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<Submesh*> submeshesList;

	std::ofstream file;
	file.open(OSPath.c_str(), std::ios::binary | std::ios::out);

	std::string uuid = this->UUID;
	uint32_t uuidSize = uuid.size();
	file.write((const char*)&uuidSize, sizeof(uint32_t));
	file.write(uuid.data(), uuidSize);

	uint32_t size = scene->mNumMeshes;
	file.write((const char*)&size, sizeof(uint32_t));

	for (int index = 0; index < scene->mNumMeshes; index++) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		aiMesh* mesh = scene->mMeshes[index];

		for (int i = 0; i < mesh->mNumVertices; i++) {
			int indice = i;
			Vertex vertex;
			vertex.Position = glm::vec3(mesh->mVertices[indice].x, mesh->mVertices[indice].y, mesh->mVertices[indice].z);
			if (mesh->HasTextureCoords(0)) {
				vertex.TexCoord = glm::vec2(mesh->mTextureCoords[0][indice].x, 1.0f - mesh->mTextureCoords[0][indice].y);
			}
			else {
				vertex.TexCoord = glm::vec2(0, 0);
			}
			if (mesh->HasNormals()) {
				vertex.Normal = glm::vec4(mesh->mNormals[indice].x, mesh->mNormals[indice].y, mesh->mNormals[indice].z, 0);
			}
			else {
				vertex.Normal = glm::vec4(0, 0, 0,0);
			}

			vertices.push_back(vertex);
		}
		for (int i = 0; i < mesh->mNumFaces; i++) {
			indices.push_back(mesh->mFaces[i].mIndices[2]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[0]);
		}

		Submesh* submesh = new Submesh();
		submesh->Init(vertices, indices);
		submesh->Name = mesh->mName.C_Str();


		nlohmann::json details;
		details["Name"] = submesh->Name;
		details["VerticesSize"] = vertices.size();
		details["IndicesSize"] = indices.size();
		std::string jsonString = details.dump();
		uint32_t jsonSize = jsonString.size();
		file.write((const char*)&jsonSize, sizeof(uint32_t));
		file.write(jsonString.data(), jsonSize);

		for (Vertex vertex : vertices) {
			file.write((const char*)&vertex, sizeof(Vertex));
		}
		for (uint32_t index : indices) {
			file.write((const char*)&index, sizeof(uint32_t));
		}

		submeshesList.push_back(submesh);
	}

	file.close();

	Mesh::Create(&Handle, submeshesList);
}

void MeshAsset::CreateEmpty()
{
	
}

void MeshAsset::ImportFromEngineType()
{
	std::ifstream infile;
	infile.open(OSPath.c_str(), std::ios::binary);

	infile.seekg(0);

	uint32_t uuidSize;
	infile.read((char*)&uuidSize, sizeof(uint32_t));
	std::string uuid;
	uuid.resize(uuidSize);
	infile.read(uuid.data(), uuidSize);


	this->UUID = uuid;
	this->Name = std::filesystem::u8path(OSPath).filename().string().substr(0, std::filesystem::u8path(OSPath).filename().string().find_last_of("."));

	uint32_t size;
	infile.read((char*)&size, sizeof(uint32_t));

	std::vector<Submesh*> submeshesList;

	for (int i = 0; i < size; i++) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

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
			uint32_t d;
			infile.read((char*)&d, sizeof(uint32_t));
			indices.push_back(d);
		}
		Submesh* submesh = new Submesh();
		submesh->Init(vertices, indices);
		submesh->Name = json["Name"];
		submeshesList.push_back(submesh);
	}

	Mesh::Create(&Handle, submeshesList);
}

std::string MeshAsset::GetType()
{
	return "Mesh";
}

std::any MeshAsset::GetHandle()
{
	return this->Handle;
}

void MeshAsset::WriteEngineFormatToStream(std::ofstream stream, std::string BinaryPath)
{

}

void MeshAsset::Save()
{
}
