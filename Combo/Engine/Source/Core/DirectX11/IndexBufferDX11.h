#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/IndexBuffer.h>

class IndexBufferDX11 : public IndexBuffer {
public:
	ID3D11Buffer* Buffer;
	virtual void Init(std::vector<uint32_t> Indices);
	virtual std::string GetApiName();
};
#endif