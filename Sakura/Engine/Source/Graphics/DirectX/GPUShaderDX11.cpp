#include "GPUShaderDX11.h"

sakura_ptr<GPUShader> GPUShader::Create(sakura_string shader)
{
	sakura_ptr<GPUShader> pShader = make_shared<GPUShader>();
	pShader->Initalize(shader);
	return pShader;
}

void GPUShader::Initalize(sakura_string text)
{
	const char* textC = text.c_str();
	D3DX11CompileFromMemory(textC, strlen(textC), nullptr, nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &this->VertexShaderBlob, nullptr, nullptr);
	D3DX11CompileFromMemory(textC, strlen(textC), nullptr, nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &this->PixelShaderBlob, nullptr, nullptr);

	GPU::Instance->Device->CreateVertexShader(this->VertexShaderBlob->GetBufferPointer(), this->VertexShaderBlob->GetBufferSize(), NULL, &this->VertexShader);
	GPU::Instance->Device->CreatePixelShader(this->PixelShaderBlob->GetBufferPointer(), this->PixelShaderBlob->GetBufferSize(), NULL, &this->PixelShader);

	GPU::Instance->Context->VSSetShader(this->VertexShader, 0, 0);
	GPU::Instance->Context->PSSetShader(this->PixelShader, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	GPU::Instance->Device->CreateInputLayout(ied, 2, this->VertexShaderBlob->GetBufferPointer(), this->PixelShaderBlob->GetBufferSize(), &this->Layout);
	GPU::Instance->Context->IASetInputLayout(this->Layout);

	GPU::Instance->Context->VSSetShader(0, 0, 0);
	GPU::Instance->Context->PSSetShader(0, 0, 0);
}
