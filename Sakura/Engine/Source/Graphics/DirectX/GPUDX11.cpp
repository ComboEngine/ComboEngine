#pragma once
#include "GPUDX11.h"
#include <Engine/Engine.h>

sakura_ptr<GPU> GPU::Instance = NULL;

sakura_ptr<GPU> GPU::Create()
{
	sakura_ptr<GPU> gpu = make_shared<GPU>();

	gpu->Initalize();

	return gpu;
}

void GPU::Initalize()
{
    //Swapchain description
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = Platform::window->Width;
    scd.BufferDesc.Height = Platform::window->Height;
    scd.OutputWindow = Platform::window->Hwnd;              // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used                              // the window to be used
    scd.SampleDesc.Count = 8;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //Create device and swapchain and check the card does support dx11
    D3D_FEATURE_LEVEL featureLevel;
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &SwapChain, &Device, &featureLevel, &Context);
    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        Engine::logger.Info("Your card doesn't support D3D11");
        Engine::RequestExit();
    }
    else {
        Engine::logger.Info("Your card does support D3D11");
    }

    //Create backbuffer texture
    ID3D11Texture2D* BackBufferTexture;
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

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

    //Submit viewport to rasterizer
    Context->RSSetViewports(1, &Viewport);

    D3D11_RASTERIZER_DESC wfdesc2;
    ZeroMemory(&wfdesc2, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc2.FillMode = D3D11_FILL_SOLID;
    wfdesc2.CullMode = D3D11_CULL_NONE;
    Device->CreateRasterizerState(&wfdesc2, &this->Solid);

    Context->RSSetState(this->Solid);
    
    //Create RenderPass
    this->RenderPass = make_shared<GPURenderPass>();
}

sakura_ptr<Mesh> GPU::CreateMesh(Vertex vertices[], int vertexCount, DWORD indices[], int indicesSize)
{
    sakura_ptr<Mesh> mesh = make_shared<Mesh>();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    this->Device->CreateBuffer(&bd, NULL, &mesh->VertexBuffer);

    D3D11_MAPPED_SUBRESOURCE ms;
    this->Context->Map(mesh->VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, vertices, vertexCount);
    this->Context->Unmap(mesh->VertexBuffer, NULL);

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 8;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
  

    this->Device->CreateBuffer(&indexBufferDesc, &iinitData, &mesh->IndexBuffer);



    mesh->VertexCount = vertexCount;

    return mesh;
}

void GPU::SubmitData(sakura_ptr<Mesh> mesh, sakura_ptr<Material> material)
{
    GPURenderData data;
    data.Mesh = mesh;
    data.Material = material;
    this->RenderPass->RenderDataList.push_back(data);
}

void GPU::Release()
{
    SwapChain->Release();
    Device->Release();
    Context->Release();
}
