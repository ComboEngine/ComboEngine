#pragma once

#include <Platform/Windows/IncludeDirectXHeaders.h>

class Mesh {
public:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	ID3D11Buffer* ConstantBuffer;
	int VertexCount;
};
struct ConstantBufferPass {
	XMMATRIX WVP;
};