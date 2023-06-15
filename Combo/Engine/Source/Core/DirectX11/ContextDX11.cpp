#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "ContextDX11.h"
#include <Core/Core.h>
#include "WindowDX11.h"


void ContextDX11::Init()
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = Core::s_Window.Cast<WindowDX11>()->hWnd;
	SwapChainDesc.SampleDesc.Count = 4;
	SwapChainDesc.Windowed = TRUE;

	CB_CHECKHR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &Context));

	ID3D11Texture2D* BackBufferTexture;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

	CB_CHECKHR(Device->CreateRenderTargetView(BackBufferTexture, NULL, &RenderTargetView));
	BackBufferTexture->Release();

	D3D11_VIEWPORT ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(ViewportDesc));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = Core::s_Window.Get()->GetWidth();
	ViewportDesc.Height = Core::s_Window.Get()->GetHeight();
	Context->RSSetViewports(1, &ViewportDesc);
}
void ContextDX11::Update()
{
	float color[] = { 0.0f,0.0f,1.0f,1.0f };
	Context->ClearRenderTargetView(RenderTargetView, color);
	SwapChain->Present(1, 0);
}
std::string ContextDX11::GetApiName()
{
	return "DX11";
}
#endif