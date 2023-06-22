#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "FramebufferDX11.h"
#include "ContextDX11.h"
#include "../Core.h"

void FramebufferDX11::Bind(bool depth)
{
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    context->Context->OMSetRenderTargets(1, &RenderTargetView,depth ? DepthStencilView : NULL);
    context->Context->ClearRenderTargetView(RenderTargetView, context->ClearColor);
    if (depth) {
        context->Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
    context->BoundFramebuffer = true;
}

void FramebufferDX11::Unbind()
{
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    context->BoundFramebuffer = false;
}

void FramebufferDX11::Init()
{
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
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

    D3D11_TEXTURE2D_DESC DepthStencilDesc;
    DepthStencilDesc.Width = Core::s_Window->GetWidth();
    DepthStencilDesc.Height = Core::s_Window->GetHeight();
    DepthStencilDesc.MipLevels = 1;
    DepthStencilDesc.ArraySize = 1;
    DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthStencilDesc.SampleDesc.Count = 1;
    DepthStencilDesc.SampleDesc.Quality = 0;
    DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthStencilDesc.CPUAccessFlags = 0;
    DepthStencilDesc.MiscFlags = 0;
    CB_CHECKHR(context->Device->CreateTexture2D(&DepthStencilDesc, NULL, &DepthStencilBuffer));
    CB_CHECKHR(context->Device->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView));
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