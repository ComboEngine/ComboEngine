#include "Engine.h"
#include <Platform/Platform.h>
#include <Scripting/Scripting.h>
#include <Graphics/GPU.h>
#include <Graphics/GPUShader.h>
#include <Renderer/Vertex.h>
#include <Renderer/Material.h>
#include <fstream>
#include <Graphics/GPU2D.h>


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
	Engine::Color = GPUFramebuffer::Create(Platform::window->Width,Platform::window->Height);

	Scripting::Init();
	World::Init();

	sakura_ptr<Actor> actor = make_shared<Actor>();
	actor->Scripts.push_back(Scripting::Scripts[0]);
	World::Actors.push_back(actor);

	Vertex vertices[] =
	{
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f),
		Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,1.0f),
		Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,1.0f),
		Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,1.0f),
		Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f,1.0f),
		Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f),
		Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f,1.0f),
		Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f),
	};

	DWORD indices[] = {
		// front face
		   0, 1, 2,
		   0, 2, 3,

		   // back face
		   4, 6, 5,
		   4, 7, 6,

		   // left face
		   4, 5, 1,
		   4, 1, 0,

		   // right face
		   3, 2, 6,
		   3, 6, 7,

		   // top face
		   1, 5, 6,
		   1, 6, 2,

		   // bottom face
		   4, 0, 3,
		   4, 3, 7
	};

	sakura_ptr<Mesh> mesh = GPU::Instance->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
	sakura_ptr<Material> material = Material::Create();
	material->Init();

	OnStart();
	while (!ShouldExit()) {
		OnUpdate();
		GPU::Instance->SubmitData(mesh, material);

		GPU::Instance->RenderPass->Render(false,Engine::Color);
		OnDraw();

		GPU::Instance->RenderPass->Render(false);
		GPU2D::RenderQuad(0, 0, Platform::window->Width, 40, Color32::From255Values(24, 24, 24, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(0, 40, 200, 1080 - 40, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(1920 - 200, 40, 200, 1080 - 40, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(0, 1080 - 200, 1920, 1080, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderFramebuffer(200, 40, 1920 - 400, 1080 - 40 - 200, Engine::Color, GPU::Instance->RenderPass);
		Engine::Color->RendererWidth = 1920 - 400;
		Engine::Color->RendererHeight = 1080 - 40 - 200;
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
	for (sakura_ptr<Actor> actor : World::Actors) {
		for (sakura_ptr<Script> script : actor->Scripts) {
			script->Draw();
		}
	}
}

void Engine::OnExit()
{
	GPU::Instance->Release();
}
