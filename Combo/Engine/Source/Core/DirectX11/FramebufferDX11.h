#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Framebuffer.h>

class FramebufferDX11 : public Framebuffer {
public:
	bool BoundWithDepth = false;
	ID3D11Texture2D* RenderTargetTexture;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11ShaderResourceView* ShaderResourceView;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11Texture2D* DepthStencilBuffer;
	virtual void Bind(bool depth);
	virtual void Unbind();
	virtual void Init();
	virtual void* GetImage();
	virtual std::string GetApiName();
};
#endif