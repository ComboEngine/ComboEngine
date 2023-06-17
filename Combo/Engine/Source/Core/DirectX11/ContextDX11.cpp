#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "ContextDX11.h"
#include <Core/Core.h>
#include "WindowDX11.h"
#include "ShaderDX11.h"
#include "VertexBufferDX11.h"
#include "IndexBufferDX11.h"
#include "ShaderDataBufferDX11.h"
#include "TextureDX11.h"
#include "FramebufferDX11.h"


void ContextDX11::Init()
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = (HWND)Core::s_Window.Get()->GetPlainWindow();
	SwapChainDesc.SampleDesc.Count = 4;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	CB_CHECKHR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &Context));

	ID3D11Texture2D* BackBufferTexture;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Core::s_Window.Get()->GetWidth();
	depthStencilDesc.Height = Core::s_Window.Get()->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	Device->CreateTexture2D(&depthStencilDesc, NULL, &DepthStencilBuffer);
	Device->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);

	CB_CHECKHR(Device->CreateRenderTargetView(BackBufferTexture, NULL, &RenderTargetView));
	BackBufferTexture->Release();

	Context->OMSetRenderTargets(1, &RenderTargetView, NULL);


	D3D11_VIEWPORT ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(ViewportDesc));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = Core::s_Window.Get()->GetWidth();
	ViewportDesc.Height = Core::s_Window.Get()->GetHeight();
	ViewportDesc.MinDepth = 0.0f;
	ViewportDesc.MaxDepth = 1.0f;
	Context->RSSetViewports(1, &ViewportDesc);

	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	Device->CreateRasterizerState(&rasterDesc, &this->RasterizerState);

	Context->RSSetState(this->RasterizerState);

	Core::ExitEvent.Hook([&] {
		this->Device->Release();
		this->Context->Release();
		this->SwapChain->Release();
		this->RenderTargetView->Release();
		this->DepthStencilBuffer->Release();
		this->DepthStencilView->Release();
	});
}
void ContextDX11::BeginDraw(Scope<Framebuffer> framebuffer)
{
	FramebufferDX11* framebufferDX11 = framebuffer.Cast<FramebufferDX11>();
	if (framebuffer.Get() == nullptr) {
		Context->ClearRenderTargetView(RenderTargetView, this->ClearColor);
		Context->OMSetRenderTargets(1, &RenderTargetView, NULL);
		//Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else {
		Context->OMSetRenderTargets(1, &framebufferDX11->RenderTargetView, DepthStencilView);
		Context->ClearRenderTargetView(framebufferDX11->RenderTargetView, this->ClearColor);
		Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
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
	TextureDX11* texture = pipeline.Texture.Cast<TextureDX11>();

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
	if (pipeline.Texture.Get()) {
		Context->PSSetShaderResources(0, 1, &texture->ShaderResourceView);
		Context->PSSetSamplers(0, 1, &texture->Sampler);
	}
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