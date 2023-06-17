#include "pch.h"
#include "Core.h"
#include "Pipeline.h"
#include "Shader.h"
#include "ShaderDataBuffer.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Assets/MeshSerializer.h"
#include "Input.h"
#include "GlobalShaders.h"
#include "Assets/TextureSerializer.h"
#include "Camera.h"


Scope<Window> Core::s_Window;
Scope<Platform> Core::s_Platform;
RendererAPI Core::RendererType = Null;
Scope<Context> Core::s_Context;
Scope<Scripting> Core::s_Scripting;

Event Core::UpdateEvent;
Event Core::DrawEvent;
Event Core::BeginPlayEvent;
Event Core::ExitEvent;

std::vector<Scope<Actor>> Core::Actors;

bool Core::ShouldExit = false;

struct ConstantBufferData {
	XMMATRIX WVP;
};

int Core::Init()
{
	//Select Renderer API
#ifdef COMBO_DIRECTX11
	Core::RendererType = DirectX11;
#endif

	CB_ASSERT(Core::RendererType != Null, "Renderer API cannot be null!");

	WindowSpecification WindowSpec;
	WindowSpec.Title = "Core";
	Window::Create(s_Window, WindowSpec);

	UpdateEvent.Hook([&] {
		s_Window.Get()->Update();
	});

	ContextSpecification ContextSpec;
	Context::Create(s_Context, ContextSpec);

	Scripting::Create(s_Scripting);

	UpdateEvent.Hook([&] {
		s_Context.Get()->BeginDraw();
		DrawEvent.Invoke();
		s_Context.Get()->EndDraw();
	});

	GlobalShaders::Init();

	Scope<Actor> actor;
	Actor::Create(actor);

	Scope<Renderer> renderer;
	Scope<Renderer>::Create(renderer);

	actor.Get()->AddComponent(renderer.Cast<Component>());

	Scope<Mesh> mesh;
	MeshSerializer::Read(mesh, "Cube.cbmesh");

	renderer.Get()->mesh = mesh;

	UpdateEvent.Hook([&] { Camera::Drone(); });

	
	Scope<Texture> test;
	TextureSerializer::Read(test, "test.cbtexture");

	Scope<Material> material;
	Material::Create(material);
	//material.Get()->Diffuse = MaterialColor::FromTexture(test);
	material.Get()->Diffuse = MaterialColor::FromColor(glm::vec4(1, 1, 1, 1));

	renderer.Get()->material = material;

	DrawEvent.Hook([&] {
		s_Context.Get()->SetClearColor(glm::vec3(0, 0, 0));
		for (Scope<Actor> actor : Actors) {
			for (Component* component : actor.Get()->Components) {
				component->Draw(actor);
			}
		}
	});

	UpdateEvent.Hook([&] {
		for (Scope<Actor> actor : Actors) {
			for (Component* component : actor.Get()->Components) {
				component->Update(actor);
			}
		}
	});

	BeginPlayEvent.Invoke();
	while (!ShouldExit) {
		UpdateEvent.Invoke();
		Input::LastMousePosition = Input::CurrentMousePosition;
	}
	ExitEvent.Invoke();
	return 0;
}

void Core::RequestExit()
{
	ShouldExit = true;
}