#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/GBuffer.h>

class GBufferDX11 : public GBuffer {
public:
	virtual void Bind();
	virtual void Unbind();
};
#endif