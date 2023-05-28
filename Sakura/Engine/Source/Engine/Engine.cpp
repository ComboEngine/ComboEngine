#include "Engine.h"
#include <Platform/Platform.h>
#include <Renderer/GPU.h>


//Engine entry point
int Engine::Main(sakura_array<sakura_string> args)
{
	logger.Info("Initalizing Sakura Engine " + Engine::version.GetVersionString());

	Platform::Init();
	GPUContext::Instance = GPUContext::CreateContext();
	

	OnStart();
	while (!ShouldExit()) {
		OnUpdate();
		OnDraw();
	}
	OnExit();
	return 0;
}

bool Engine::ShouldExit() {
	return Engine::shouldExit;
}

void Engine::RequestExit()
{
	//Exit from loop
	Engine::shouldExit = true;
}

void Engine::OnStart()
{
}

void Engine::OnUpdate()
{
	Platform::OnUpdate();
}

void Engine::OnDraw()
{
	GPUContext::Instance->D3D11DevCon->ClearRenderTargetView(GPUContext::Instance->RenderTargetView, D3DXCOLOR(1,0,1,1));

	//Present the backbuffer to the screen
	GPUContext::Instance->SwapChain->Present(0, 0);
}

void Engine::OnExit()
{
	GPUContext::Instance->Release();
}
