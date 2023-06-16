#include "pch.h"
#ifdef COMBO_DIRECTX11
#include "ShaderDX11.h"
#include "ContextDX11.h"
#include <Core/Core.h>


void ShaderDX11::Init(std::string VertexSource, std::string PixelSource)
{
	std::wstring vertex_source(VertexSource.begin(), VertexSource.end());
	std::wstring pixel_source(PixelSource.begin(), PixelSource.end());
	ContextDX11* context = Core::s_Context.Cast<ContextDX11>();
	D3DCompileFromFile(vertex_source.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_4_0_level_9_1", NULL, NULL, &VertexBlob, NULL);
	D3DCompileFromFile(pixel_source.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_4_0_level_9_1", NULL, NULL, &PixelBlob, NULL);

	CB_ASSERT(VertexBlob != nullptr, "Failed to compile Vertex Shader!");
	CB_ASSERT(PixelBlob != nullptr, "Failed to compile Pixel Shader!");

	CB_CHECKHR(context->Device->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), NULL, &VertexShader));
	CB_CHECKHR(context->Device->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), NULL, &PixelShader));

	context->Context->VSSetShader(VertexShader, 0, 0);
	context->Context->PSSetShader(PixelShader, 0, 0);

	D3D11_INPUT_ELEMENT_DESC InputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	CB_CHECKHR(context->Device->CreateInputLayout(InputElementDesc, 3, VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &InputLayout));

	context->Context->VSSetShader(0, 0, 0);
	context->Context->PSSetShader(0, 0, 0);

	Core::ExitEvent.Hook([&] {
		this->PixelShader->Release();
		this->VertexShader->Release();
		this->PixelBlob->Release();
		this->VertexBlob->Release();
		this->InputLayout->Release();
	});
}
std::string ShaderDX11::GetApiName()
{
	return "DX11";
}
#endif