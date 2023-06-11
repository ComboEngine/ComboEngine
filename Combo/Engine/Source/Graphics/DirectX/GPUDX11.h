#pragma once


#include <Platform/Platform.h>
#include <pch.h>
#include "GPURenderPassDX11.h"
#include <Graphics/Mesh.h>
#include "../GPURenderData.h"
#include <Renderer/Vertex.h>


class GPURenderPass;
class GPUShader;
class GPUFramebuffer;
class Transform;
class Material;
class GPU {
public:
	static std::shared_ptr<GPU> Instance;
	static std::shared_ptr<GPU> Create();

	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11RenderTargetView* Backbuffer;
	ID3D11RasterizerState* Solid;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11Texture2D* DepthStencilBuffer;

	std::shared_ptr<GPURenderPass> RenderPass;
	std::vector<std::shared_ptr<GPUFramebuffer>> Framebuffers = std::vector<std::shared_ptr<GPUFramebuffer>>();

	void Initalize();
	std::shared_ptr<Submesh> CreateSubmesh(std::vector<Vertex> vertices,std::vector<DWORD> indices);
	void SubmitData(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,std::shared_ptr<Transform> transform);
	void Release();
};