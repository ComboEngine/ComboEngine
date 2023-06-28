#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "TextureDX11.h"
#include "ContextDX11.h"
#include "../Core.h"
#include "FramebufferDX11.h"

void TextureDX11::Init(void* mipData, int Width, int Height)
{
	ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mipData;
	InitData.SysMemPitch = Width * 4;

	CB_CHECKHR(context->Device->CreateTexture2D(&TextureDesc, &InitData, &this->Texture2D));
	CB_CHECKHR(context->Device->CreateShaderResourceView(this->Texture2D, NULL, &this->ShaderResourceView));

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

	CB_CHECKHR(context->Device->CreateSamplerState(&SamplerDesc, &this->Sampler));
}

void TextureDX11::ReadFromFramebuffer(Framebuffer* framebuffer)
{
	FramebufferDX11* buffer = reinterpret_cast<FramebufferDX11*>(framebuffer);
	this->ShaderResourceView = buffer->ShaderResourceView;
	this->Texture2D = buffer->RenderTargetTexture;
}

std::string TextureDX11::GetApiName()
{
	return "DX11";
}

void* TextureDX11::GetImGuiImage()
{
	return ShaderResourceView;
}

#endif