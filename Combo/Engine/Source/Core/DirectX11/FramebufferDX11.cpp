#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "FramebufferDX11.h"
#include "ContextDX11.h"
#include "../Core.h"

void FramebufferDX11::Init()
{
    ContextDX11* context = Core::s_Context.Cast<ContextDX11>();
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    textureDesc.Width = this->Width;
    textureDesc.Height = this->Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    CB_CHECKHR(context->Device->CreateTexture2D(&textureDesc, NULL, &this->RenderTargetTexture));

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    CB_CHECKHR(context->Device->CreateRenderTargetView(this->RenderTargetTexture, &renderTargetViewDesc, &this->RenderTargetView));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    CB_CHECKHR(context->Device->CreateShaderResourceView(this->RenderTargetTexture, &shaderResourceViewDesc, &this->ShaderResourceView));
}
void* FramebufferDX11::GetImage()
{
    return this->ShaderResourceView;
}
std::string FramebufferDX11::GetApiName()
{
	return "DX11";
}
#endif