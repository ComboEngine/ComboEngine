#pragma once
#include <Renderer/Vertex.h>


class Submesh {
public:
	std::string Name;
	std::vector<Vertex> Vertices;
	std::vector<DWORD> Indices;
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	ID3D11Buffer* ConstantBuffer;
	int VertexCount;
};
class Mesh {
public:
	std::vector<std::shared_ptr<Submesh>> Submeshes;
};
struct ConstantBufferPass {
	XMMATRIX WVP;
};