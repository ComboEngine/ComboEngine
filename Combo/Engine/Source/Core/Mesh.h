#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderDataBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Assets/Asset.h"
#include "RenderStages.h"

struct MeshShaderData {
	glm::mat4 WVP;
	glm::mat4 Model;
	glm::mat4 RotationMatrix;
	glm::vec4 Diffuse;
	glm::vec4 DiffuseUseTexture;
};

class Submesh {
public:
	VertexBuffer* VertexBuffer;
	IndexBuffer* IndexBuffer;
	Asset* Material;
	std::string Name;
	int Count;
	void Init(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices);
};

class Mesh {
public:
	static void Create(Mesh** Obj, std::vector<Submesh*> Submeshes);
	void Render(Material* Mat, glm::vec3 Position, glm::quat Orientation, glm::vec3 Scale);
	std::vector<Submesh*> Submeshes;
	ShaderDataBuffer* ShaderDataBuffer;
};