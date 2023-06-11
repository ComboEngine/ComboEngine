#pragma once

#include <Math/Color.h>
#include <map>
#include "GPUShaderDX11.h"

class GPU2D {
public:
	static ID3D11SamplerState* TextureSampler;
	static std::shared_ptr<GPUShader> QuadShader;
	static std::shared_ptr<GPUShader> TexturedQuadShader;
	static std::shared_ptr<Submesh> QuadMesh;
	static void Init();
	static void RenderQuad(float x,float y,float width,float height,Color32 color, std::shared_ptr<GPURenderPass> renderPass);
	static void RenderFramebuffer(float x, float y, float width, float height, std::shared_ptr<GPUFramebuffer> color, std::shared_ptr<GPURenderPass> renderPass);
};

struct ConstantBufferPass2D {
	XMMATRIX WVP;
	XMFLOAT4 Color;
};