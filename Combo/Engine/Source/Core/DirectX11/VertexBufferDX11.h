#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/VertexBuffer.h>

class VertexBufferDX11 : public VertexBuffer {
public:
	ID3D11Buffer* Buffer;
	virtual void Init(std::vector<Vertex> Vertices);
	virtual std::string GetApiName();
};
#endif