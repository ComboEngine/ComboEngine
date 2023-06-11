#include "pch.h"
#include "GPU2DDX11.h"


std::shared_ptr<GPUShader> GPU2D::QuadShader = nullptr;
std::shared_ptr<GPUShader> GPU2D::TexturedQuadShader = nullptr;
std::shared_ptr<Submesh> GPU2D::QuadMesh = nullptr;
ID3D11SamplerState* GPU2D::TextureSampler = nullptr;

void GPU2D::Init()
{
    QuadShader = GPUShader::Create(
        "cbuffer ConstantBuffer2D \n"
        "{\n"
        "    float4x4 WVP;\n"
        "    float4 Color;\n"
        "};\n"
        "struct VOut\n"
        "{\n"
        "    float4 position : SV_POSITION;\n"
        "    float2 texCoord : TEXCOORD;\n"
        "	 float4 normal : NORMAL; \n"
        "};\n"
        "\n"
        "VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)\n"
        "{\n"
        "    VOut output;\n"
        "\n"
        "    output.position = mul(position,WVP);\n"
        "    output.texCoord = float2(texCoord.x,-texCoord.y);\n"
        "    output.normal = normal;\n"
        "\n"
        "    return output;\n"
        "}\n"
        "\n"
        "\n"
        "\n"
        "float4 PShader(VOut data) : SV_TARGET {\n"
        "   return Color;\n"
        "}");

    TexturedQuadShader = GPUShader::Create(
        "cbuffer ConstantBuffer2D \n"
        "{\n"
        "    float4x4 WVP;\n"
        "    float4 Color;\n"
        "};\n"
        "struct VOut\n"
        "{\n"
        "    float4 position : SV_POSITION;\n"
        "    float2 texCoord : TEXCOORD;\n"
        "	 float4 normal : NORMAL; \n"
        "};\n"
        "\n"
        "VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 normal : NORMAL)\n"
        "{\n"
        "    VOut output;\n"
        "\n"
        "    output.position = mul(position,WVP);\n"
        "    output.texCoord = float2(texCoord.x,-texCoord.y);\n"
        "    output.normal = normal;\n"
        "\n"
        "    return output;\n"
        "}\n"
        "\n"
        "\n"
        "\n"
        "Texture2D Texture;\n"
        "SamplerState TextureSampler;\n"
        "float4 PShader(VOut data) : SV_TARGET {\n"
        "   //return float4(data.texCoord,1,1);\n"
        "   return Texture.Sample(TextureSampler, data.texCoord);\n"
        "}");

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    GPU::Instance->Device->CreateSamplerState(&sampDesc, &TextureSampler);

    Vertex vList[] = {
        {  0.0f, -0.0f, 0.0f ,  0.0f, 1.0f ,  0.0f, 0.0f, 1.0f  },    // Vertex 0
        {  1.0f, -0.0f, 0.0f ,  1.0f, 1.0f ,  0.0f, 0.0f, 1.0f  },     // Vertex 1
        {  1.0f, 1.0f, 0.0f ,  1.0f, 0.0f ,  0.0f, 0.0f, 1.0f  },      // Vertex 2
        {  0.0f, 1.0f, 0.0f ,  0.0f, 0.0f ,  0.0f, 0.0f, 1.0f }      // Vertex 3
    };

    DWORD iList[] = {
        0, 1, 2,
        0, 2, 3
    };

    QuadMesh = std::make_shared<Submesh>();


    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GPU::Instance->Device->CreateBuffer(&bd, NULL, &QuadMesh->VertexBuffer);

    D3D11_MAPPED_SUBRESOURCE ms;
    GPU::Instance->Context->Map(QuadMesh->VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, vList, sizeof(vList));
    GPU::Instance->Context->Unmap(QuadMesh->VertexBuffer, NULL);

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * 18 * 8;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = iList;

    GPU::Instance->Device->CreateBuffer(&indexBufferDesc, &iinitData, &QuadMesh->IndexBuffer);

    D3D11_BUFFER_DESC ConstantBufferDesc;
    ZeroMemory(&ConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ConstantBufferDesc.ByteWidth = sizeof(ConstantBufferPass2D) * 16;
    ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ConstantBufferDesc.CPUAccessFlags = 0;
    ConstantBufferDesc.MiscFlags = 0;

    GPU::Instance->Device->CreateBuffer(&ConstantBufferDesc, NULL, &QuadMesh->ConstantBuffer);

    std::cout << "Created 2D Graphics Context" << std::endl;
}

void GPU2D::RenderQuad(float x, float y, float width, float height, Color32 color,std::shared_ptr<GPURenderPass> renderPass)
{
    if (QuadShader == nullptr) {
        Init();
    }

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ConstantBufferPass2D pass;
    if (renderPass->ActiveFramebuffer != nullptr) {
        pass.WVP = XMMatrixScaling(width, height, 1) * XMMatrixTranslation(x, y, 0) * XMMatrixOrthographicOffCenterRH(0, renderPass->ActiveFramebuffer->RendererWidth, renderPass->ActiveFramebuffer->RendererHeight, 0, 1, -1);
    }
    else {
        pass.WVP = XMMatrixScaling(width, height, 1) * XMMatrixTranslation(x, y, 0) * XMMatrixOrthographicOffCenterLH(0, (float)Platform::window->Width, (float)Platform::window->Height, 0, 0.0f, 1.0f);
    }

    pass.WVP = XMMatrixTranspose(pass.WVP);
    pass.Color = XMFLOAT4(color.R, color.G, color.B, color.A);
    GPU::Instance->Context->VSSetShader(QuadShader->VertexShader, 0, 0);
    GPU::Instance->Context->PSSetShader(QuadShader->PixelShader, 0, 0);
    GPU::Instance->Context->UpdateSubresource(QuadMesh->ConstantBuffer, 0, NULL, &pass, 0, 0);
    GPU::Instance->Context->VSSetConstantBuffers(0, 1, &QuadMesh->ConstantBuffer);
    GPU::Instance->Context->PSSetConstantBuffers(0, 1, &QuadMesh->ConstantBuffer);
    GPU::Instance->Context->IASetIndexBuffer(QuadMesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    GPU::Instance->Context->IASetVertexBuffers(0, 1, &QuadMesh->VertexBuffer, &stride, &offset);
    GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    GPU::Instance->Context->DrawIndexed(6, 0, 0);

}

void GPU2D::RenderFramebuffer(float x, float y, float width, float height, std::shared_ptr<GPUFramebuffer> color, std::shared_ptr<GPURenderPass> renderPass)
{
    if (QuadShader == nullptr) {
        Init();
    }

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ConstantBufferPass2D pass;
    if (renderPass->ActiveFramebuffer != nullptr) {
        pass.WVP = XMMatrixScaling(width, height, 1) * XMMatrixOrthographicOffCenterRH(0, renderPass->ActiveFramebuffer->RendererWidth, renderPass->ActiveFramebuffer->RendererHeight, 0, 1, -1);
    }
    else {
        pass.WVP = XMMatrixScaling(width, height, 1) * XMMatrixTranslation(x, y, 0) * XMMatrixOrthographicOffCenterLH(0, (float)Platform::window->Width, (float)Platform::window->Height, 0, 0.0f, 1.0f);
    }

    pass.WVP = XMMatrixTranspose(pass.WVP);
    pass.Color = XMFLOAT4(1,1,1,1);
    GPU::Instance->Context->VSSetShader(TexturedQuadShader->VertexShader, 0, 0);
    GPU::Instance->Context->PSSetShader(TexturedQuadShader->PixelShader, 0, 0);
    GPU::Instance->Context->UpdateSubresource(QuadMesh->ConstantBuffer, 0, NULL, &pass, 0, 0);
    GPU::Instance->Context->PSSetSamplers(0, 1, &TextureSampler);
    GPU::Instance->Context->PSSetShaderResources(0, 1, &color->ShaderResourceView);
    GPU::Instance->Context->VSSetConstantBuffers(0, 1, &QuadMesh->ConstantBuffer);
    GPU::Instance->Context->PSSetConstantBuffers(0, 1, &QuadMesh->ConstantBuffer);
    GPU::Instance->Context->IASetIndexBuffer(QuadMesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    GPU::Instance->Context->IASetVertexBuffers(0, 1, &QuadMesh->VertexBuffer, &stride, &offset);
    GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    GPU::Instance->Context->DrawIndexed(6, 0, 0);
}
