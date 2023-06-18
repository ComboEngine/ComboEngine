#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Framebuffer.h>

class FramebufferDX11 : public Framebuffer {
public:
	ID3D11Texture2D* RenderTargetTexture;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11ShaderResourceView* ShaderResourceView;
	virtual void Init();
	virtual void* GetImage();
	virtual std::string GetApiName();
};
#endif