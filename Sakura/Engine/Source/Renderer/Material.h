#pragma once

#include <Engine/EngineHeaders.h>
#include <Math/Color.h>
#include <Graphics/GPUShader.h>


class GPUShader;
class Material {
public:
	Color32 albedo = Color32(1,1,1,1);
	sakura_ptr<GPUShader> shader;
	void Init();
	static sakura_ptr<Material> Create();
	sakura_string Compile();
};