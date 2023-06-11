#include "pch.h"
#include "GPURenderPassDX11.h"
#include "GPUDX11.h"
#include "GPUImGuiDX11.h"
#include <World/World.h>


float rotation = 0;

XMMATRIX ConvertToXMMATRIX(const glm::mat4& matrix)
{
	const float* glmMatrixData = glm::value_ptr(matrix);
	XMFLOAT4X4 xnaMatrix;
	XMStoreFloat4x4(&xnaMatrix,XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(glmMatrixData)));
	return XMLoadFloat4x4(&xnaMatrix);
}

void GPURenderPass::Render(bool RenderImGui,std::shared_ptr<GPUFramebuffer> framebuffer)
{
	this->ActiveFramebuffer = framebuffer;
	glm::mat4 projection;
	glm::mat4 camera = World::GetCamera()->CalculateMatrix();
	if (framebuffer == nullptr) {
		GPU::Instance->Context->ClearRenderTargetView(GPU::Instance->Backbuffer, this->Color.GetD3DXColor());
		GPU::Instance->Context->OMSetRenderTargets(1, &GPU::Instance->Backbuffer, NULL);
		GPU::Instance->Context->ClearDepthStencilView(GPU::Instance->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		projection = glm::perspective(0.4f * 3.14f, (float)Platform::window->Width / Platform::window->Height, 1.0f, 1000.0f);
	}
	else {
		GPU::Instance->Context->OMSetRenderTargets(1, &framebuffer->RenderTargetView, GPU::Instance->DepthStencilView);
		GPU::Instance->Context->ClearRenderTargetView(framebuffer->RenderTargetView, this->Color.GetD3DXColor());
		GPU::Instance->Context->ClearDepthStencilView(GPU::Instance->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		projection = glm::perspective(0.4f * 3.14f, (float)framebuffer->RendererWidth / framebuffer->RendererHeight, 1.0f, 1000.0f);
	}
	for (GPURenderData data : this->RenderDataList) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		for (std::shared_ptr<Submesh> submesh : data.Mesh->Submeshes) {
			ConstantBufferPass pass;
			rotation += 0.05f;

			pass.WVP = ConvertToXMMATRIX(projection * camera * data.Matrix);
			pass.WVP = XMMatrixTranspose(pass.WVP);
			GPU::Instance->Context->VSSetShader(data.Material->shader->VertexShader, 0, 0);
			GPU::Instance->Context->PSSetShader(data.Material->shader->PixelShader, 0, 0);
			GPU::Instance->Context->UpdateSubresource(submesh->ConstantBuffer, 0, NULL, &pass, 0, 0);
			GPU::Instance->Context->VSSetConstantBuffers(0, 1, &submesh->ConstantBuffer);
			GPU::Instance->Context->PSSetConstantBuffers(0, 1, &submesh->ConstantBuffer);
			GPU::Instance->Context->IASetIndexBuffer(submesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			GPU::Instance->Context->IASetVertexBuffers(0, 1, &submesh->VertexBuffer, &stride, &offset);
			GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			if (data.Material->texture != nullptr) {
				GPU::Instance->Context->PSSetShaderResources(0, 1, &data.Material->texture->ShaderResourceView);
				GPU::Instance->Context->PSSetSamplers(0, 1, &data.Material->texture->SamplerState);
			}
			GPU::Instance->Context->DrawIndexed(submesh->VertexCount, 0, 0);
		}
	}
	this->RenderDataList.clear();
	if (RenderImGui) {
		GPUImGuiDX11::Render();
	}
}

void GPURenderPass::SubmitToScreen(bool vSync)
{
	GPU::Instance->SwapChain->Present(vSync ? 1 :0, 0);
}
