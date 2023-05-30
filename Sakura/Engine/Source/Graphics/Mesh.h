#pragma once

#include <Platform/Windows/IncludeDirectXHeaders.h>

class Mesh {
public:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexCount;
};