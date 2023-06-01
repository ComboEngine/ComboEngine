#include "Engine.h"
#include <Platform/Platform.h>
#include <Scripting/Scripting.h>
#include <Graphics/GPU.h>
#include <Graphics/GPUShader.h>
#include <Renderer/Vertex.h>
#include <Renderer/Material.h>
#include <fstream>


/*
* 	AssetManager::InitAssetManager();
	sakura_ptr<MaterialAsset> materialasset = AssetManager::GetAsset<MaterialAsset>(AssetManager::GetUUIDByName("TestMaterial"));
	sakura_ptr<MeshAsset> meshasset = AssetManager::GetAsset<MeshAsset>(AssetManager::GetUUIDByName("TestMesh"));

	AssetManager::SaveAssetPack("test.sakura");
	AssetManager::LoadAssetPack("test.sakura");*/

//Engine entry point
int Engine::Main(sakura_array<sakura_string> args)
{
	logger.Info("Initalizing Sakura Engine " + Engine::version.GetVersionString());

	Platform::Init();
	GPU::Instance = GPU::Create();
	Engine::Color = GPUFramebuffer::Create();

	Scripting::Init();
	World::Init();

	sakura_ptr<Actor> actor = make_shared<Actor>();
	actor->Scripts.push_back(Scripting::Scripts[0]);
	World::Actors.push_back(actor);

	Vertex vertices[] =
	{
		Vertex(-0.5f, -0.5f, 0.5f,1.0f,1.0f,1.0f,1.0f,1.0f),
		Vertex(-0.5f,  0.5f, 0.5f,1.0f,0.0f,1.0f,1.0f,1.0f),
		Vertex(0.5f,  0.5f, 0.5f,0.0f,0.0f,1.0f,1.0f,1.0f),
		Vertex(0.5f, -0.5f, 0.5f,0.0f,1.0f,1.0f,1.0f,1.0f),
	};

	DWORD indices[] = {
	0, 1, 2,
	0, 2, 3,
	};

	sakura_ptr<Mesh> mesh = GPU::Instance->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
	sakura_ptr<Material> material = Material::Create();
	material->Init();

	OnStart();
	while (!ShouldExit()) {
		OnUpdate();
		//OnDraw();
		GPU::Instance->SubmitData(mesh, material);
		GPU::Instance->RenderPass->Render(false,Engine::Color);

		GPU::Instance->RenderPass->Render();
		GPU::Instance->RenderPass->SubmitToScreen(true);
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
	GPU::Instance->RenderPass->Render();
	GPU::Instance->RenderPass->SubmitToScreen(true);
}

void Engine::OnExit()
{
	GPU::Instance->Release();
}
