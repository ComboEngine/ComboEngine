#include "pch.h"
#ifdef COMBO_DIRECTX11
#include <Core/Window.h>
#include "WindowDX11.h"
#include "PlatformDX11.h"
#include <Core/Core.h>
#include "ContextDX11.h"


LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		Core::RequestExit();
		return 0;
	case WM_CLOSE:
		Core::RequestExit();
		return 0;
	case WM_QUIT:
		Core::RequestExit();
		return 0;
	case WM_SIZE:
		ContextDX11* context = Core::s_Context.Cast<ContextDX11>();
		if (context != nullptr && context->SwapChain != nullptr)
		{
			context->RenderTargetView->Release();
			context->SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			ID3D11Texture2D* BackBufferTexture;
			context->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

			CB_CHECKHR(context->Device->CreateRenderTargetView(BackBufferTexture, NULL, &context->RenderTargetView));
			BackBufferTexture->Release();

			D3D11_VIEWPORT ViewportDesc;
			ZeroMemory(&ViewportDesc, sizeof(ViewportDesc));
			ViewportDesc.TopLeftX = 0;
			ViewportDesc.TopLeftY = 0;
			ViewportDesc.Width = Core::s_Window.Get()->GetWidth();
			ViewportDesc.Height = Core::s_Window.Get()->GetHeight();
			context->Context->RSSetViewports(1, &ViewportDesc);
		}
		return 1;
	}

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

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
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		if (Message.message == WM_QUIT)
		{
			Core::RequestExit();
		}
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