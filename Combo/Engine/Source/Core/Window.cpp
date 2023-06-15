#include "pch.h"
#include "Window.h"
#include "Core.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/WindowDX11.h>
#endif

void Window::Create(Scope<Window>& window, WindowSpecification Specification)
{
	window = Scope<Window>();
	//Select window api

#ifdef COMBO_DIRECTX11
	window.Set(new WindowDX11());
#endif
	window.Get()->Specification = Specification;

	

	//Call API Window
	window.Get()->Init();

	LOG(std::string("Created Window (" + Specification.Title + ", " + std::to_string(Specification.Width) + ", " + std::to_string(Specification.Height) + ")"))
}
