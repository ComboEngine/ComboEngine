#pragma once

#include <Platform/Windows/IncludeDirectXHeaders.h>

class Mesh {
public:
	ID3D11Buffer* VertexBuffer;
	int VertexCount;
};