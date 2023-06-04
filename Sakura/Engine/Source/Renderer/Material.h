#pragma once

#include <pch.h>
#include <Math/Color.h>
#include <Graphics/GPUShader.h>
#include <Graphics/GPUTexture.h>


class GPUShader;
class Material {
public:
	Color32 albedo = Color32(1,1,1,1);
	std::shared_ptr<GPUTexture> texture = nullptr;
	std::shared_ptr<GPUShader> shader;
	void Init();
	static std::shared_ptr<Material> Create();
	std::string Compile();
};