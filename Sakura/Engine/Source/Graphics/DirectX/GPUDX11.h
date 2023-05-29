#pragma once

#include <Platform/Windows/IncludeDirectXHeaders.h>
#include <Platform/Platform.h>
#include <Engine/EngineHeaders.h>
#include "GPURenderPassDX11.h"
#include <Graphics/Mesh.h>
#include <Renderer/Vertex.h>

class GPU {
public:
	static sakura_ptr<GPU> Instance;
	static sakura_ptr<GPU> Create();

	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11RenderTargetView* Backbuffer;
	ID3D11RasterizerState* Solid;

	sakura_ptr<GPURenderPass> RenderPass;

	void Initalize();
	sakura_ptr<Mesh> CreateMesh(Vertex vertices[], int vertexCount);
	void Release();
};