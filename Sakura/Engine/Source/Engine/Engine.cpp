#include "Engine.h"
#include <Platform/Platform.h>
#include <Scripting/Scripting.h>
#include <Graphics/GPU.h>
#include <Graphics/GPUShader.h>
#include <Renderer/Vertex.h>
#include <Renderer/Material.h>
#include <fstream>


//Engine entry point
int Engine::Main(sakura_array<sakura_string> args)
{
	logger.Info("Initalizing Sakura Engine " + Engine::version.GetVersionString());

	Platform::Init();
	GPU::Instance = GPU::Create();
	
	Scripting::Init();
	World::Init();

	//Setup my little project :)
	
	/*Vertex v[] =
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};
	sakura_ptr<Material> material = Material::Create();
	sakura_ptr<Mesh> mesh = Mesh::Create(v);
	*/


	Vertex vertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.5f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.5f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	sakura_ptr<Actor> actor = make_shared<Actor>();
	actor->Scripts.push_back(Scripting::Scripts[0]);
	World::Actors.push_back(actor);


	sakura_string shaderStr = "struct VOut\n" 
		"{\n" 
		"    float4 position : SV_POSITION;\n" 
		"    float4 color : COLOR;\n" 
		"};\n" 
		"\n" 
		"VOut VShader(float4 position : POSITION, float4 color : COLOR)\n" 
		"{\n" 
		"    VOut output;\n" 
		"\n" 
		"    output.position = position;\n" 
		"    output.color = color;\n" 
		"\n" 
		"    return output;\n" 
		"}\n" 
		"\n" 
		"\n" 
		"float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET\n" 
		"{\n" 
		"    return color;\n" 
		"}";

	sakura_ptr<GPUShader> shader = GPUShader::Create(shaderStr);
	sakura_ptr<Mesh> mesh = GPU::Instance->CreateMesh(vertices, sizeof(vertices));


	OnStart();
	while (!ShouldExit()) {
		OnUpdate();
		OnDraw();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		GPU::Instance->Context->VSSetShader(shader->VertexShader, 0, 0);
		GPU::Instance->Context->PSSetShader(shader->PixelShader, 0, 0);
		GPU::Instance->Context->IASetVertexBuffers(0, 1, &mesh->VertexBuffer, &stride, &offset);
		GPU::Instance->Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GPU::Instance->Context->Draw(mesh->VertexCount, 0);
		GPU::Instance->RenderPass->End();
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
	GPU::Instance->RenderPass->Start();
}

void Engine::OnExit()
{
	GPU::Instance->Release();
}
