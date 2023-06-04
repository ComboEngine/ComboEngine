#pragma once


#include <pch.h>
#include "GPUDX11.h"

class GPUShader {
public:
	ID3D10Blob* VertexShaderBlob;
	ID3D10Blob* PixelShaderBlob;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* Layout;
	static std::shared_ptr<GPUShader> Create(std::string shader);

	void Initalize(std::string text);
};