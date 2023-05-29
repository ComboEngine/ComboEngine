#pragma once


#include <Engine/EngineHeaders.h>
#include "GPUDX11.h"

class GPUShader {
public:
	ID3D10Blob* VertexShaderBlob;
	ID3D10Blob* PixelShaderBlob;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* Layout;
	static sakura_ptr<GPUShader> Create(sakura_string shader);

	void Initalize(sakura_string text);
};