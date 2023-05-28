#pragma once
#include <Engine/EngineHeaders.h>

class GPUContext {
public:
	static sakura_ptr<GPUContext> Instance;
	IDXGISwapChain* SwapChain;
	ID3D11Device* D3D11Device;
	ID3D11DeviceContext* D3D11DevCon;
	ID3D11RenderTargetView* RenderTargetView;
	static sakura_ptr<GPUContext> CreateContext();
	void Release();
};