#include "Engine.h"
#include <Platform/Platform.h>
#include <Renderer/GPU.h>
#include <Scripting/Scripting.h>


//Engine entry point
int Engine::Main(sakura_array<sakura_string> args)
{
	logger.Info("Initalizing Sakura Engine " + Engine::version.GetVersionString());

	Platform::Init();
	GPUContext::Instance = GPUContext::CreateContext();
	
	Scripting::Init();
	World::Init();


	//Setup my little project :)
	sakura_ptr<Actor> actor = make_shared<Actor>();
	actor->Scripts.push_back(Scripting::Scripts[0]);
	World::Actors.push_back(actor);

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
	//Platform update
	Platform::OnUpdate();


	for (sakura_ptr<Actor> actor : World::Actors) {
		for (sakura_ptr<Script> script : actor->Scripts) {
			script->Update();
		}
	}
}

void Engine::OnDraw()
{
	GPUContext::Instance->D3D11DevCon->ClearRenderTargetView(GPUContext::Instance->RenderTargetView, D3DXCOLOR(0,0,0,0));

	GPUContext::Instance->SwapChain->Present(1, 0);
}

void Engine::OnExit()
{
	GPUContext::Instance->Release();
}
