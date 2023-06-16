#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderDataBuffer.h"
#include "Shader.h"

struct MeshShaderData {
	XMMATRIX WVP;
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
	void Render(Scope<Shader> shader);
	std::vector<Submesh> Submeshes;
	Scope<ShaderDataBuffer> ShaderDataBuffer;
};