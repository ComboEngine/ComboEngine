#include "pch.h"
#include "Core.h"
#include "Window.h"

Scope<Window> Core::s_Window;
Scope<Platform> Core::s_Platform;
RendererAPI Core::RendererType = Null;


int Core::Init()
{
	//Select Renderer API
#ifdef COMBO_DIRECTX11
	Core::RendererType = DirectX11;
#endif

	WindowSpecification WindowSpec;
	Window::Create(s_Window, WindowSpec);

	int updateCount = 0;
	while (true) {
		s_Window.Get()->Update();
	}
	return 0;
}
