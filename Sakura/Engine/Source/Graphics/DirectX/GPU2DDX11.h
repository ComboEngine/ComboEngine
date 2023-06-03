#pragma once

#include <Math/Color.h>
#include <map>
#include "GPUShaderDX11.h"

class GPU2D {
public:
	static ID3D11SamplerState* TextureSampler;
	static sakura_ptr<GPUShader> QuadShader;
	static sakura_ptr<GPUShader> TexturedQuadShader;
	static sakura_ptr<Mesh> QuadMesh;
	static void Init();
	static void RenderQuad(float x,float y,float width,float height,Color32 color, sakura_ptr<GPURenderPass> renderPass);
	static void RenderFramebuffer(float x, float y, float width, float height, sakura_ptr<GPUFramebuffer> color, sakura_ptr<GPURenderPass> renderPass);
};

struct ConstantBufferPass2D {
	XMMATRIX WVP;
	XMFLOAT4 Color;
};