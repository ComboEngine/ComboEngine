#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderDataBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

struct MeshShaderData {
	XMMATRIX WVP;
	XMFLOAT4 Diffuse;
	int DiffuseUseTexture;
};

class Submesh {
public:
	Scope<VertexBuffer> VertexBuffer;
	Scope<IndexBuffer> IndexBuffer;
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
	std::string Name;
	int Count;
	void Init(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices);
};

class Mesh {
public:
	static void Create(Scope<Mesh>& Obj, std::vector<Submesh> Submeshes);
	void Render(Scope<Material> Mat, glm::vec3 Position, glm::quat Orientation, glm::vec3 Scale);
	std::vector<Submesh> Submeshes;
	Scope<ShaderDataBuffer> ShaderDataBuffer;
};