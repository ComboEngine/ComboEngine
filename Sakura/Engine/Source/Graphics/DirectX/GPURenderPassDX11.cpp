#include "GPURenderPassDX11.h"

#include "GPUDX11.h"

void GPURenderPass::Start()
{
	GPU::Instance->Context->ClearRenderTargetView(GPU::Instance->Backbuffer, this->Color.GetD3DXColor());

}

void GPURenderPass::End()
{
	GPU::Instance->SwapChain->Present(0, 0);
}
