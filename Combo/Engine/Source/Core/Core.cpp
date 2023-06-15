#include "pch.h"
#include "Core.h"
#include "Pipeline.h"
#include "Shader.h"

Scope<Window> Core::s_Window;
Scope<Platform> Core::s_Platform;
RendererAPI Core::RendererType = Null;
Scope<Context> Core::s_Context;


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

	ContextSpecification ContextSpec;
	Context::Create(s_Context, ContextSpec);

	Scope<Shader> testShader;
	Shader::Create(testShader, "./shader.hlsl", "./shader.hlsl");

	Vertex vertices[] = {
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
	VertexBuffer::Create(testVertices, std::vector<Vertex>(vertices,vertices + (sizeof(vertices) / sizeof(vertices[0]))));
	IndexBuffer::Create(testIndices, indices);

	while (true) {
		s_Window.Get()->Update();
		s_Context.Get()->BeginDraw();

		Pipeline pipeline;
		pipeline.Count = 6;
		pipeline.Indexed = true;
		pipeline.Shader = testShader;
		pipeline.VertexBuffer = testVertices;
		pipeline.IndexBuffer = testIndices;
		s_Context.Get()->Draw(pipeline);

		s_Context.Get()->EndDraw();
	}
	return 0;
}
