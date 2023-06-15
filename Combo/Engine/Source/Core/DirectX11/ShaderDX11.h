#ifdef COMBO_DIRECTX11
#pragma once
#include <Core/Shader.h>

class ShaderDX11 : public Shader {
public:
	ID3DBlob* VertexBlob;
	ID3DBlob* PixelBlob;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;
	virtual void Init(std::string VertexSource, std::string PixelSource);
	virtual std::string GetApiName();
};
#endif