#include "pch.h"
#include "GPUFramebufferDX11.h"

std::shared_ptr<GPUFramebuffer> GPUFramebuffer::Create(float width,float height)
{
	std::shared_ptr<GPUFramebuffer> framebuffer = std::make_shared<GPUFramebuffer>();
	framebuffer->Init(width,height);
	return framebuffer;
}

void GPUFramebuffer::Init(float width,float height)
{
    this->RendererWidth = width;
    this->RendererHeight = height;
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    GPU::Instance->Device->CreateTexture2D(&textureDesc, NULL, &this->RenderTargetTexture);

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    GPU::Instance->Device->CreateRenderTargetView(this->RenderTargetTexture, &renderTargetViewDesc, &this->RenderTargetView);

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    GPU::Instance->Device->CreateShaderResourceView(this->RenderTargetTexture, &shaderResourceViewDesc, &this->ShaderResourceView);
}
