#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/Window.h>
#include "WindowDX11.h"
#include "PlatformDX11.h"
#include <Core/Core.h>

void WindowDX11::Init()
{
	std::wstring window_name(Specification.Title.begin(), Specification.Title.end());

	PlatformDX11* platform = Core::s_Platform.Cast<PlatformDX11>();
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = platform->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = window_name.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	this->hWnd = CreateWindowEx(NULL, window_name.c_str(), window_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Specification.Width, Specification.Height, NULL, NULL, platform->hInstance, NULL);
	
	ShowWindow(this->hWnd, SW_SHOW);
}

void WindowDX11::Update() {
	if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		if (Message.message == WM_QUIT)
			//Engine::RequestExit();
			return;
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

int WindowDX11::GetWidth()
{
	RECT rect;
	GetWindowRect(this->hWnd, &rect);
	return rect.right - rect.left;
}

int WindowDX11::GetHeight()
{
	RECT rect;
	GetWindowRect(this->hWnd, &rect);
	return rect.bottom - rect.top;
}

const void* WindowDX11::GetPlainWindow()
{
	return this->hWnd;
}

std::string WindowDX11::GetApiName()
{
	return "DX11";
}


#endif