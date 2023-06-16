#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "ContextDX11.h"
#include <Core/Core.h>
#include "WindowDX11.h"
#include "ShaderDX11.h"
#include "VertexBufferDX11.h"
#include "IndexBufferDX11.h"
#include "ShaderDataBufferDX11.h"


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

	Core::ExitEvent.Hook([&] {
		this->Device->Release();
		this->Context->Release();
		this->SwapChain->Release();
		this->RenderTargetView->Release();
	});
}
void ContextDX11::BeginDraw()
{
	Context->OMSetRenderTargets(1, &RenderTargetView, NULL);
	Context->ClearRenderTargetView(RenderTargetView, this->ClearColor);
}
void ContextDX11::EndDraw()
{
	SwapChain->Present(1, 0);
}
void ContextDX11::Draw(Pipeline pipeline)
{
	ShaderDX11* shader = pipeline.Shader.Cast<ShaderDX11>();
	VertexBufferDX11* vertexBuffer = pipeline.VertexBuffer.Cast<VertexBufferDX11>();
	IndexBufferDX11* indexBuffer = pipeline.IndexBuffer.Cast<IndexBufferDX11>();
	ShaderDataBufferDX11* shaderDataBuffer = pipeline.ShaderDataBuffer.Cast<ShaderDataBufferDX11>();

	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;

	Context->VSSetShader(shader->VertexShader, 0, 0);
	Context->PSSetShader(shader->PixelShader, 0, 0);
	Context->VSSetConstantBuffers(0, 1, &shaderDataBuffer->Buffer);
	Context->PSSetConstantBuffers(0, 1, &shaderDataBuffer->Buffer);
	Context->IASetInputLayout(shader->InputLayout);
	Context->IASetVertexBuffers(0, 1, &vertexBuffer->Buffer, &Stride,&Offset);
	if (pipeline.Indexed) {
		Context->IASetIndexBuffer(indexBuffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
	}
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (pipeline.Indexed) {
		Context->DrawIndexed(pipeline.Count, 0, 0);
	}
	else {
		Context->Draw(pipeline.Count, 0);
	}
}

void ContextDX11::SetClearColor(glm::vec3 color)
{
	this->ClearColor[0] = color.x;
	this->ClearColor[1] = color.y;
	this->ClearColor[2] = color.z;
	this->ClearColor[3] = 1;
}

std::string ContextDX11::GetApiName()
{
	return "DX11";
}
#endif