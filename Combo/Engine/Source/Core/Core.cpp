#include "pch.h"
#include "Core.h"
#include "Assets/MeshSerializer.h"
#include "GlobalShaders.h"
#include "ImGui.h"
#include "Input.h"
#include "Assets/SceneSerializer.h"


Scope<Window> Core::s_Window;
Scope<Platform> Core::s_Platform;
RendererAPI Core::RendererType = Null;
Scope<Context> Core::s_Context;
Scope<Scripting> Core::s_Scripting;
Scope<Framebuffer> Core::s_Color;

Event Core::UpdateEvent;
Event Core::DrawEvent;
Event Core::BeginPlayEvent;
Event Core::ExitEvent;
Event Core::ImGuiDrawEvent;

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

	ImGuiAdapter::Init();

	Scope<Framebuffer> EmptyFramebuffer;
	EmptyFramebuffer.Set(nullptr);

	UpdateEvent.Hook([&] {
		s_Context.Get()->BeginDraw(s_Color);
		DrawEvent.Invoke();

		s_Context.Get()->BeginDraw(EmptyFramebuffer);
		ImGuiAdapter::StartFrame();
		ImGuiDrawEvent.Invoke();
		ImGuiAdapter::EndFrame();
		s_Context.Get()->EndDraw();
	});

	GlobalShaders::Init();

	Framebuffer::Create(s_Color, s_Window.Get()->GetWidth(), s_Window.Get()->GetHeight(), FramebufferTarget::Color);

	SceneSerializer::Load("test.cbscene");

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