#include "Engine.h"
#include "pch.h"
#include <Platform/Platform.h>
#include <Scripting/Scripting.h>
#include <Graphics/GPU.h>
#include <Graphics/GPUShader.h>
#include <Renderer/Vertex.h>
#include <Renderer/Material.h>
#include <fstream>
#include <Graphics/GPU2D.h>
#include <Utility/AssimpModelImporter.h>
#include <Graphics/GPUTexture.h>
#include <Platform/Input.h>


/*
* 	AssetManager::InitAssetManager();
	std::shared_ptr<MaterialAsset> materialasset = AssetManager::GetAsset<MaterialAsset>(AssetManager::GetUUIDByName("TestMaterial"));
	std::shared_ptr<MeshAsset> meshasset = AssetManager::GetAsset<MeshAsset>(AssetManager::GetUUIDByName("TestMesh"));

	AssetManager::SaveAssetPack("test.Combo");
	AssetManager::LoadAssetPack("test.Combo");*/

//Engine entry point
int Engine::Main(std::vector<std::string> args)
{
	logger.Info("Initalizing Combo Engine " + Engine::version.GetVersionString());

	Platform::Init();
	logger.Info("Initalized Platform");
	GPU::Instance = GPU::Create();
	logger.Info("Created GPU");
	Engine::Color = GPUFramebuffer::Create(Platform::window->Width,Platform::window->Height);
	logger.Info("Created Color Framebuffer");

	Scripting::Init();
	logger.Info("Initalized Scripting");
	World::Init();
	logger.Info("Initalized World");

	std::shared_ptr<Actor> actor = Actor::Create();
	actor->Scripts.push_back(Scripting::Scripts[0]);

	
	std::shared_ptr<Mesh> mesh = AssimpModelImporter::LoadMesh("scena.obj");
	std::shared_ptr<Material> material = Material::Create();
	std::shared_ptr<GPUTexture> texture = GPUTexture::Create("scena.png");
	//material->texture = texture;
	material->texture = nullptr;
	material->albedo = Color32(1, 0,0, 1);
	material->Init();


	float delta = 0;
	OnStart();
	logger.Info("Entered into loop");
	while (!ShouldExit()) {
		OnUpdate();
		delta += 1;
		World::camera->transform->Position = glm::vec3(0, 5,20);
		World::camera->transform->Orientation = glm::quat(glm::vec3(glm::radians(-10.0f), 0, 0));
		GPU::Instance->SubmitData(mesh, material,actor->GetTransform());
		GPU::Instance->RenderPass->Render(false,Engine::Color);
		OnDraw();

		GPU::Instance->RenderPass->Render(false);
		GPU2D::RenderQuad(0, 0, Platform::window->Width, 40, Color32::From255Values(24, 24, 24, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(0, 40, 200, Platform::window->Height - 40, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(Platform::window->Width - 200, 40, 200, Platform::window->Height - 40, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderQuad(0, Platform::window->Height - 200, Platform::window->Width, Platform::window->Height, Color32::From255Values(18, 18, 18, 255), GPU::Instance->RenderPass);
		GPU2D::RenderFramebuffer(200, 40, Platform::window->Width - 400, Platform::window->Height - 40 - 200, Engine::Color, GPU::Instance->RenderPass);
		Engine::Color->RendererWidth = Platform::window->Width - 400;
		Engine::Color->RendererHeight = Platform::window->Height - 40 - 200;
		GPU::Instance->RenderPass->SubmitToScreen(true);

		Input::End();
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
	World::Update();

	for (std::shared_ptr<Actor> actor : World::Actors) {
		for (std::shared_ptr<Script> script : actor->Scripts) {
			script->Update();
		}
	}

}

void Engine::OnDraw()
{
	for (std::shared_ptr<Actor> actor : World::Actors) {
		for (std::shared_ptr<Script> script : actor->Scripts) {
			script->Draw();
		}
	}
}

void Engine::OnExit()
{
	GPU::Instance->Release();
}
