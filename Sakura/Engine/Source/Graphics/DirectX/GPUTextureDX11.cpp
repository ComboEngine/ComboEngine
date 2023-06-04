#include "pch.h"
#include "GPUTextureDX11.h"
#include "GPUDX11.h"

std::shared_ptr<GPUTexture> GPUTexture::Create(std::string path)
{
	std::shared_ptr<GPUTexture> texture = std::make_shared<GPUTexture>();

	D3DX11CreateShaderResourceViewFromFileA(GPU::Instance->Device, path.c_str(),
		NULL, NULL, &texture->ShaderResourceView, NULL);


	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	GPU::Instance->Device->CreateSamplerState(&sampDesc, &texture->SamplerState);

	ID3D11Texture2D* pTextureInterface = 0;
	ID3D11Resource* res;
	texture->ShaderResourceView->GetResource(&res);
	res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	texture->Width = desc.Width;
	texture->Height = desc.Height;
	return texture;
}
