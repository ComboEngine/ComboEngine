#include "pch.h"
#include "Window.h"
#include "Core.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/WindowDX11.h>
#endif

void Window::Create(Window** window, WindowSpecification Specification)
{
	//Select window api

#ifdef COMBO_DIRECTX11
	*window = new WindowDX11();
#endif

	Window* windowPtr = *window;
	windowPtr->Specification = Specification;

	

	//Call API Window
	windowPtr->Init();

	LOG(std::string("Created Window (" + Specification.Title + ", " + std::to_string(Specification.Width) + ", " + std::to_string(Specification.Height) + ")"))
}
