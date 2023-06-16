#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/ShaderDataBuffer.h>

class ShaderDataBufferDX11 : public ShaderDataBuffer {
public:
	ID3D11Buffer* Buffer;
	virtual void Init(size_t Size);
	virtual void Update(const void* Data);
	virtual std::string GetApiName();
};
#endif