#include "GPURenderPassDX11.h"

#include "GPUDX11.h"
#include "GPUImGuiDX11.h"

void GPURenderPass::Start()
{
	GPU::Instance->Context->ClearRenderTargetView(GPU::Instance->Backbuffer, this->Color.GetD3DXColor());
	for (GPURenderData data : this->RenderDataList) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		GPU::Instance->Context->VSSetShader(data.Material->shader->VertexShader, 0, 0);
		GPU::Instance->Context->PSSetShader(data.Material->shader->PixelShader, 0, 0);
		GPU::Instance->Context->IASetIndexBuffer(data.Mesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		GPU::Instance->Context->IASetVertexBuffers(0, 1, &data.Mesh->VertexBuffer, &stride, &offset);
		GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GPU::Instance->Context->DrawIndexed(data.Mesh->VertexCount, 0, 0);
	}
	this->RenderDataList.clear();
	if (RenderImGui) {
		GPUImGuiDX11::Render();
	}
}

void GPURenderPass::End()
{
	GPU::Instance->SwapChain->Present(1, 0);
}
