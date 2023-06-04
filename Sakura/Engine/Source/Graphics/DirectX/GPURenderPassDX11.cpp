#include "pch.h"
#include "GPURenderPassDX11.h"
#include "GPUDX11.h"
#include "GPUImGuiDX11.h"

float rotation = 0;

void GPURenderPass::Render(bool RenderImGui,std::shared_ptr<GPUFramebuffer> framebuffer)
{
	this->ActiveFramebuffer = framebuffer;
	XMMATRIX projection;
	if (framebuffer == nullptr) {
		GPU::Instance->Context->ClearRenderTargetView(GPU::Instance->Backbuffer, this->Color.GetD3DXColor());
		GPU::Instance->Context->OMSetRenderTargets(1, &GPU::Instance->Backbuffer, NULL);
		GPU::Instance->Context->ClearDepthStencilView(GPU::Instance->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)Platform::window->Width / Platform::window->Height, 1.0f, 1000.0f);
	}
	else {
		GPU::Instance->Context->OMSetRenderTargets(1, &framebuffer->RenderTargetView, GPU::Instance->DepthStencilView);
		GPU::Instance->Context->ClearRenderTargetView(framebuffer->RenderTargetView, this->Color.GetD3DXColor());
		GPU::Instance->Context->ClearDepthStencilView(GPU::Instance->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)framebuffer->RendererWidth / framebuffer->RendererHeight, 1.0f, 1000.0f);
	}
	for (GPURenderData data : this->RenderDataList) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		for (std::shared_ptr<Submesh> submesh : data.Mesh->Submeshes) {
			ConstantBufferPass pass;
			rotation += 0.05f;
			pass.WVP = data.Matrix * XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * projection;
			pass.WVP = XMMatrixTranspose(pass.WVP);
			GPU::Instance->Context->VSSetShader(data.Material->shader->VertexShader, 0, 0);
			GPU::Instance->Context->PSSetShader(data.Material->shader->PixelShader, 0, 0);
			GPU::Instance->Context->UpdateSubresource(submesh->ConstantBuffer, 0, NULL, &pass, 0, 0);
			GPU::Instance->Context->VSSetConstantBuffers(0, 1, &submesh->ConstantBuffer);
			GPU::Instance->Context->PSSetConstantBuffers(0, 1, &submesh->ConstantBuffer);
			GPU::Instance->Context->IASetIndexBuffer(submesh->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			GPU::Instance->Context->IASetVertexBuffers(0, 1, &submesh->VertexBuffer, &stride, &offset);
			GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			GPU::Instance->Context->PSSetShaderResources(0, 1, &data.Material->texture->ShaderResourceView);
			GPU::Instance->Context->PSSetSamplers(0, 1, &data.Material->texture->SamplerState);
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
