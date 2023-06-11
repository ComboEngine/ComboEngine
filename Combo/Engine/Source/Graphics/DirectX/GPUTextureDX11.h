#pragma once

#include <pch.h>


class GPUTexture {
public:
	ID3D11ShaderResourceView* ShaderResourceView;
	ID3D11SamplerState* SamplerState;
	int Width;
	int Height;
	static std::shared_ptr<GPUTexture> Create(std::string path);
};