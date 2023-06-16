#include "pch.h"
#include "Core.h"
#include "Pipeline.h"
#include "Shader.h"

Scope<Window> Core::s_Window;
Scope<Platform> Core::s_Platform;
RendererAPI Core::RendererType = Null;
Scope<Context> Core::s_Context;

Event Core::UpdateEvent;
Event Core::DrawEvent;
Event Core::BeginPlayEvent;
Event Core::ExitEvent;

bool Core::ShouldExit = false;

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

	UpdateEvent.Hook([&] {
		s_Context.Get()->BeginDraw();
		DrawEvent.Invoke();
		s_Context.Get()->EndDraw();
	});

	Scope<Shader> testShader;
	Shader::Create(testShader, "./shader.hlsl", "./shader.hlsl");

	std::vector<Vertex> vertices = {
		{-0.5f,  -0.5f, 1.0f, 1.0f,0.0f,0.0f,1.0f},
		{-0.5f,   0.5f, 1.0f, 0.0f,1.0f,0.0f,1.0f},
		{ 0.5f,   0.5f, 1.0f, 0.0f,0.0f,1.0f,1.0f},
		{ 0.5f,  -0.5f, 1.0f, 1.0f,0.0f,1.0f,1.0f}
	};

	std::vector<uint32_t> indices = {
		0,1,2,0,2,3
	};

	Scope<VertexBuffer> testVertices;
	Scope<IndexBuffer> testIndices;

	VertexBuffer::Create(testVertices, vertices);
	IndexBuffer::Create(testIndices, indices);

	DrawEvent.Hook([&] {
		s_Context.Get()->SetClearColor(glm::vec3(0, 0, 0));

		Pipeline pipeline;
		pipeline.Count = testIndices.Get()->Size;
		pipeline.Indexed = true;
		pipeline.Shader = testShader;
		pipeline.VertexBuffer = testVertices;
		pipeline.IndexBuffer = testIndices;
		s_Context.Get()->Draw(pipeline);
	});

	BeginPlayEvent.Invoke();
	while (!ShouldExit) {
		UpdateEvent.Invoke();
	}
	ExitEvent.Invoke();
	return 0;
}

void Core::RequestExit()
{
	ShouldExit = true;
}