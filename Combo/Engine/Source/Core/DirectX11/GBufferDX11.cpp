#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "GBufferDX11.h"
#include "../Core.h"
#include "ContextDX11.h"
#include "TextureDX11.h"
#include "FramebufferDX11.h"

void GBufferDX11::Bind()
{
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    if (PositionTexture == nullptr) {
        D3D11_SAMPLER_DESC SamplerDesc = {};

        SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.MipLODBias = 0.0f;
        SamplerDesc.MaxAnisotropy = 1;
        SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        SamplerDesc.BorderColor[0] = 1.0f;
        SamplerDesc.BorderColor[1] = 1.0f;
        SamplerDesc.BorderColor[2] = 1.0f;
        SamplerDesc.BorderColor[3] = 1.0f;
        SamplerDesc.MinLOD = -FLT_MAX;
        SamplerDesc.MaxLOD = FLT_MAX;

        ID3D11SamplerState* state;
        context->Device->CreateSamplerState(&SamplerDesc, &state);

        PositionTexture = new TextureDX11();
        TextureDX11* dx11PositionTexture = reinterpret_cast<TextureDX11*>(PositionTexture);
        dx11PositionTexture->Sampler = state;
        DiffuseTexture = new TextureDX11();
        TextureDX11* dx11DiffuseTexture = reinterpret_cast<TextureDX11*>(DiffuseTexture);
        dx11DiffuseTexture->Sampler = state;
        NormalTexture = new TextureDX11();
        TextureDX11* dx11NormalTexture = reinterpret_cast<TextureDX11*>(NormalTexture);
        dx11NormalTexture->Sampler = state;

        PositionTexture->ReadFromFramebuffer(Position);
        DiffuseTexture->ReadFromFramebuffer(Diffuse);
        NormalTexture->ReadFromFramebuffer(Normal);
    }
    FramebufferDX11* position = reinterpret_cast<FramebufferDX11*>(Position);
    FramebufferDX11* diffuse = reinterpret_cast<FramebufferDX11*>(Diffuse);
    FramebufferDX11* normal = reinterpret_cast<FramebufferDX11*>(Normal);

    ID3D11RenderTargetView* renderTargetViews[] = {
        position->RenderTargetView,diffuse->RenderTargetView,normal->RenderTargetView
    };

    context->Context->OMSetRenderTargets(3, renderTargetViews,NULL);

    context->Context->ClearRenderTargetView(position->RenderTargetView, context->ClearColor);
    context->Context->ClearRenderTargetView(diffuse->RenderTargetView, context->ClearColor);
    context->Context->ClearRenderTargetView(normal->RenderTargetView, context->ClearColor);

    context->Context->ClearDepthStencilView(context->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context->BoundFramebuffer = true;
}

void GBufferDX11::Unbind()
{
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    context->BoundFramebuffer = false;
}

#endif