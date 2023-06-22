#include "pch.h"
#include "Core.h"
#include "GlobalShaders.h"
#include "ImGui.h"
#include "Input.h"
#include "Camera.h"
#include "Render2D.h"
#include "Mesh.h"
#ifdef COMBO_EDITOR
#include <Editor/Editor.h>
#endif


Window* Core::s_Window;
Platform* Core::s_Platform;
RendererAPI Core::RendererType = Null;
Context* Core::s_Context;
Scripting* Core::s_Scripting;
Project Core::s_Project;
Framebuffer* Core::s_Final;

GBuffer* Core::s_GBuffer;

Event Core::UpdateEvent;
Event Core::DrawEvent;
Event Core::BeginPlayEvent;
Event Core::ExitEvent;
Event Core::ImGuiDrawEvent;

std::vector<Actor*> Core::Actors;

bool Core::ShouldExit = false;

struct ConstantBufferData {
	XMMATRIX WVP;
};

int Core::Init()
{

	LOG(uuid::generate_uuid_v4());
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

	std::vector<Vertex> vertices = {
		{1.0f,1.0f,0.0f,	1.0f,1.0f,	0.0f,0.0f,0.0f,0.0f},
		{1.0f,-1.0f,0.0f,	1.0f,0.0f,	0.0f,0.0f,0.0f,0.0f},
		{-1.0f,-1.0f,0.0f,	0.0f,0.0f,	0.0f,0.0f,0.0f,0.0f},
		{-1.0f,1.0f,0.0f,	0.0f,1.0f,	0.0f,0.0f,0.0f,0.0f}
	};

	std::vector<uint32_t> indices = {
		0,1,3,1,2,3
	};

	VertexBuffer* VertexBuffer;
	IndexBuffer* IndexBuffer;
	VertexBuffer::Create(&VertexBuffer, vertices);
	IndexBuffer::Create(&IndexBuffer, indices);

	UpdateEvent.Hook([&] {
		/*s_Final->Bind(true);
		s_Context->BeginDraw();
		DrawEvent.Invoke();
		s_Final->Unbind();*/
 
		s_GBuffer->Bind();
		OPTICK_EVENT("Rendering GBuffer");
		s_Context->BeginDraw();
		DrawEvent.Invoke();
		s_GBuffer->Unbind();

		s_Final->Bind(false);
		s_Context->BeginDraw();
		Pipeline pipeline;
		pipeline.Shader = GlobalShaders::GetShader(GlobalShader::PostFX);
		pipeline.Count = 6;
		pipeline.IndexBuffer = IndexBuffer;
		pipeline.VertexBuffer = VertexBuffer;
		pipeline.Indexed = true;
		pipeline.ShaderDataBuffer = nullptr;
		pipeline.Textures = s_GBuffer->GetTextureArray();
		s_Context->Draw(pipeline);
		s_Final->Unbind();

		OPTICK_EVENT("Begin ImGui Draw");
		s_Context->BeginDraw();
		ImGuiAdapter::StartFrame();
		ImGuiDrawEvent.Invoke();
		Render2D::RenderImGui();
		ImGuiAdapter::EndFrame();
	});

	GlobalShaders::Init();

	Framebuffer::Create(&s_Final, s_Window->GetWidth(), s_Window->GetHeight(), FramebufferTarget::Color);
	GBuffer::Create(&s_GBuffer);

	Camera::ProjectionWidth = s_Window->GetWidth();
	Camera::ProjectionHeight = s_Window->GetHeight();

#ifdef COMBO_EDITOR
	Editor::Init();
#else
	SceneSerializer::Load("test.cbscene");
#endif

	DrawEvent.Hook([&] {
		s_Context->SetClearColor(glm::vec3(0, 0, 0));
		for (Actor* actor : Actors) {
			for (Component* component : actor->Components) {
				component->Draw(actor);
			}
		}
	});

	UpdateEvent.Hook([&] {
		for (Actor* actor : Actors) {
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
		Input::LastMousePosition = Input::CurrentMousePosition;
	}
	ExitEvent.Invoke();
	return 0;
}

void Core::RequestExit()
{
	ShouldExit = true;
}