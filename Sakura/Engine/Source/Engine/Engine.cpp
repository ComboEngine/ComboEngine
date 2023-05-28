#include "Engine.h"
#include <Platform/Platform.h>


//Engine entry point
int Engine::Main(sakura_array<sakura_string> args)
{
	logger.Info("Initalizing Sakura Engine " + Engine::version.GetVersionString());

	Platform::Init();
	

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
}

void Engine::OnExit()
{
}
