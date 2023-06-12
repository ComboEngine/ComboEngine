#include "pch.h"
#include "AssimpModelImporter.h"


#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#pragma comment(lib,"assimp-vc143-mtd.lib")

std::shared_ptr<Mesh> AssimpModelImporter::LoadMesh(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);


	std::vector<std::shared_ptr<Submesh>> submeshesList;

	for (int index = 0; index < scene->mNumMeshes;index++) {
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		aiMesh* mesh = scene->mMeshes[index];

		for (int i = 0; i < mesh->mNumVertices; i++) {
			int indice = i;
			Vertex vertex; 
			vertex.pos.x = mesh->mVertices[indice].x;
			vertex.pos.y = mesh->mVertices[indice].y;
			vertex.pos.z = mesh->mVertices[indice].z;
			if (mesh->HasTextureCoords(0)) {
				vertex.texCoord.x = mesh->mTextureCoords[0][indice].x;
				vertex.texCoord.y = 1.0f - mesh->mTextureCoords[0][indice].y;
			}
			else {
				vertex.texCoord.x = 0.0f;
				vertex.texCoord.y = 0.0f;
			}
			if (mesh->HasNormals()) {
				vertex.normal.x = mesh->mNormals[indice].x;
				vertex.normal.y = mesh->mNormals[indice].y;
				vertex.normal.z = mesh->mNormals[indice].z;
				vertex.normal.w = 0;
			}
			else {
				vertex.normal.x = 0;
				vertex.normal.y = 0;
				vertex.normal.z = 0;
				vertex.normal.w = 0;
			}

			vertices.push_back(vertex);
		}
		for (int i = 0; i < mesh->mNumFaces; i++) {
			indices.push_back(mesh->mFaces[i].mIndices[2]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[0]);
		}
		std::shared_ptr<Submesh> submesh = GPU::Instance->CreateSubmesh(vertices, indices);
		submesh->Name = mesh->mName.C_Str();
		submeshesList.push_back(submesh);
	}


	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->Submeshes = submeshesList;

	return mesh;
}
