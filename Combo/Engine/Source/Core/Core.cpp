#include "pch.h"
#include "Core.h"
#include "Window.h"

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

	while (true) {
		s_Window.Get()->Update();
		s_Context.Get()->Update();
	}
	return 0;
}
