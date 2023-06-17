#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Texture.h>

class TextureDX11 : public Texture {
public:
	ID3D11Texture2D* Texture2D;
	ID3D11ShaderResourceView* ShaderResourceView;
	ID3D11SamplerState* Sampler;
	virtual void Init(void* mipData, int Width, int Height);
	virtual std::string GetApiName();
};
#endif