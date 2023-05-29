#pragma once

#include <Engine/EngineHeaders.h>
#include <Math/Color.h>
//#include <Graphics/DirectX/DX11/GPUShader.h>

class GPUShader;

class Material {
public:
	Color32 albedo = Color32(1,1,1,1);
	//sakura_ptr<GPUShader> shader;
	static sakura_ptr<Material> Create();
	sakura_string GetPixelShader() {
		return "float4 PS() : SV_TARGET {\n"
			"	return float4(0.0f, 0.0f, 1.0f, 1.0f);\n"
			"}\n";
	}
};