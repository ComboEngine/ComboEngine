#pragma once
#include "pch.h"
#include "GPUDX11.h"
#include <Engine/Engine.h>
#include "GPUImGuiDX11.h"

std::shared_ptr<GPU> GPU::Instance = NULL;

std::shared_ptr<GPU> GPU::Create()
{
	std::shared_ptr<GPU> gpu = std::make_shared<GPU>();

	gpu->Initalize();

	return gpu;
}

void GPU::Initalize()
{
    //Swapchain description
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;                                  
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   
    scd.BufferDesc.Width = Platform::window->Width;
    scd.BufferDesc.Height = Platform::window->Height;
    scd.OutputWindow = Platform::window->Hwnd;              
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       
    scd.SampleDesc.Count = 8;                            
    scd.Windowed = TRUE;                                 
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //Create device and swapchain and check the card does support dx11
    D3D_FEATURE_LEVEL featureLevel;
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &SwapChain, &Device, &featureLevel, &Context);
    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        Engine::logger.Info("Your card doesn't support D3D11");
        MessageBoxW(Platform::window->Hwnd, L"Your card doesn't support D3D11", L"Combo Engine",MB_ICONERROR);
        Engine::RequestExit();
    }
    else {
        Engine::logger.Info("Your card does support D3D11");
    }

    //Create backbuffer texture
    ID3D11Texture2D* BackBufferTexture;
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = Platform::window->Width;
    depthStencilDesc.Height = Platform::window->Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    Device->CreateTexture2D(&depthStencilDesc, NULL, &DepthStencilBuffer);
    Device->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);

    //Create render target view
    Device->CreateRenderTargetView(BackBufferTexture,NULL,&Backbuffer);

    //Destroy backbuffer texture
    BackBufferTexture->Release();

    //Set our backbuffer to output merger; TODO: Add depth stencil
    Context->OMSetRenderTargets(1, &Backbuffer, NULL);

    //Setup viewport
    D3D11_VIEWPORT Viewport;
    ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
    Viewport.TopLeftX = 0;
    Viewport.TopLeftY = 0;
    Viewport.Width = Platform::window->Width;
    Viewport.Height = Platform::window->Height;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;

    //Submit viewport to rasterizer
    Context->RSSetViewports(1, &Viewport);

    D3D11_RASTERIZER_DESC wfdesc2;
    ZeroMemory(&wfdesc2, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc2.FillMode = D3D11_FILL_SOLID;
    wfdesc2.CullMode = D3D11_CULL_NONE;
    Device->CreateRasterizerState(&wfdesc2, &this->Solid);

    Context->RSSetState(this->Solid);
    
    //Create RenderPass
    this->RenderPass = std::make_shared<GPURenderPass>();

    GPUImGuiDX11::Create(this);
}

std::shared_ptr<Submesh> GPU::CreateSubmesh(std::vector<Vertex> vertices, std::vector<DWORD> indices)
{
    std::shared_ptr<Submesh> mesh = std::make_shared<Submesh>();

    mesh->Vertices = vertices;
    mesh->Indices = indices;

    mesh->VertexCount = indices.size();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * vertices.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA iiinitData;
    iiinitData.pSysMem = vertices.data();

    this->Device->CreateBuffer(&bd, &iiinitData, &mesh->VertexBuffer);

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices.data();
  
    this->Device->CreateBuffer(&indexBufferDesc, &iinitData, &mesh->IndexBuffer);

    D3D11_BUFFER_DESC ConstantBufferDesc;
    ZeroMemory(&ConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ConstantBufferDesc.ByteWidth = sizeof(ConstantBufferPass)*16;
    ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ConstantBufferDesc.CPUAccessFlags = 0;
    ConstantBufferDesc.MiscFlags = 0;

    this->Device->CreateBuffer(&ConstantBufferDesc, NULL, &mesh->ConstantBuffer);

    return mesh;
}

void GPU::SubmitData(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Transform> transform)
{
    GPURenderData data;
    data.Mesh = mesh;
    data.Material = material;
    data.Matrix = transform->CalculateMatrix();
    this->RenderPass->RenderDataList.push_back(data);
}

void GPU::Release()
{
    SwapChain->Release();
    Device->Release();
    Context->Release();
}
