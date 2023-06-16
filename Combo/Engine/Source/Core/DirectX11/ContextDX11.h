#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Context.h>

class ContextDX11 : public Context {
public:
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	virtual void Init();
	virtual void BeginDraw();
	virtual void EndDraw();
	virtual void Draw(Pipeline pipeline);
	virtual void SetClearColor(glm::vec3 color);
	virtual std::string GetApiName();
};
#endif