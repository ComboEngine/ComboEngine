#pragma once

#include "GPUDX11.h"

class GPUFramebuffer {
public:
	ID3D11Texture2D* RenderTargetTexture;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11ShaderResourceView* ShaderResourceView;
	static sakura_ptr<GPUFramebuffer> Create(float width,float height);
	float RendererWidth = 0;
	float RendererHeight = 0;
	void Init(float width,float height);
};