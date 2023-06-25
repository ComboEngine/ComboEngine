#include "pch.h"
#include "Core.h"
#include "GlobalShaders.h"
#include "ImGui.h"
#include "Input.h"
#include "Camera.h"
#include "Render2D.h"
#include "Mesh.h"
#include "Assets/ProjectSerializer.h"
#ifdef COMBO_EDITOR
#include <Editor/Editor.h>
#endif


Window* Core::s_Window;
Platform* Core::s_Platform;
RendererAPI Core::RendererType = Null;
Context* Core::s_Context;
Scripting* Core::s_Scripting;
Project Core::s_Project;

GBuffer* Core::s_GBuffer;

Event Core::UpdateEvent;
Event Core::DrawEvent;
Event Core::BeginPlayEvent;
Event Core::ExitEvent;
Event Core::ImGuiDrawEvent;

Scene Core::Scene;
PostFXRenderer Core::PostFX;

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
	Window::Create(&s_Window, WindowSpec);

	UpdateEvent.Hook([&] {
		s_Window->Update();
	});

	ContextSpecification ContextSpec;
	Context::Create(&s_Context, ContextSpec);

	Scripting::Create(&s_Scripting);

	ImGuiAdapter::Init();
	Core::s_Project.Assets["0"] = nullptr;

	PostFX.Init();

	UpdateEvent.Hook([&] {
		s_GBuffer->Bind();
		OPTICK_EVENT("Rendering GBuffer");
		s_Context->BeginDraw();
		DrawEvent.Invoke();
		s_GBuffer->Unbind();

		PostFX.Draw();

		OPTICK_EVENT("Begin ImGui Draw");
		s_Context->BeginDraw();
		ImGuiAdapter::StartFrame();
		ImGuiDrawEvent.Invoke();
		#ifndef COMBO_EDITOR
		Camera::Drone();
		Render2D::RenderTexturedRect(0, 0, s_Window->GetWidth(), s_Window->GetHeight(), PostFX.Frame->GetImage());
		s_Window->LockCursor(true);
		#endif
		Render2D::RenderImGui();
		ImGuiAdapter::EndFrame();
	});

	GlobalShaders::Init();

	GBuffer::Create(&s_GBuffer);

	Camera::ProjectionWidth = s_Window->GetWidth();
	Camera::ProjectionHeight = s_Window->GetHeight();

#ifdef COMBO_EDITOR
	Editor::Init();
#else
	ProjectSerializer::LoadProject("D:\\ComboEngine\\Sandbox\\Sandbox.cbproject");
#endif

	DrawEvent.Hook([&] {
		s_Context->SetClearColor(glm::vec3(0, 0, 0));
		for (Actor* actor : Scene.Actors) {
			for (Component* component : actor->Components) {
				component->Draw(actor);
			}
		}
	});

	UpdateEvent.Hook([&] {
		for (Actor* actor : Scene.Actors) {
			for (Component* component : actor->Components) {
				component->Update(actor);
			}
		}
	});

	BeginPlayEvent.Invoke();
	while (!ShouldExit) {
		OPTICK_FRAME("Update");
		UpdateEvent.Invoke();
		OPTICK_EVENT("Presenting");
		s_Context->EndDraw();
		Core::Scene.EndScene();
		Input::LastMousePosition = Input::CurrentMousePosition;
	}
	ExitEvent.Invoke();
	return 0;
}

void Core::RequestExit()
{
	ShouldExit = true;
}