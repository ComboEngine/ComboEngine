#include "GPUContext.h"
#include <Platform/Platform.h>

sakura_ptr<GPUContext> GPUContext::Instance = nullptr;

sakura_ptr<GPUContext> GPUContext::CreateContext()
{
	sakura_ptr<GPUContext> context = make_shared<GPUContext>();

	HRESULT hr;

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Platform::window->Width;
	bufferDesc.Height = Platform::window->Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = Platform::window->Hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &context->SwapChain, &context->D3D11Device, NULL, &context->D3D11DevCon);

	ID3D11Texture2D* BackBuffer;
	hr = context->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	hr = context->D3D11Device->CreateRenderTargetView(BackBuffer, NULL, &context->RenderTargetView);
	BackBuffer->Release();
	context->D3D11DevCon->OMSetRenderTargets(1, &context->RenderTargetView, NULL);

	return context;
}

void GPUContext::Release()
{
	SwapChain->Release();
	D3D11Device->Release();
	D3D11DevCon->Release();
}
