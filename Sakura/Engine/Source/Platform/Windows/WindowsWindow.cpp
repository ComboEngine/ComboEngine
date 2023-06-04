#include "pch.h"
#include "WindowsWindow.h"
#include "WindowsPlatform.h"
#include <Graphics/GPUImGui.h>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd,	
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	switch (msg)	
	{
		case WM_KEYDOWN:	
			break;
		case WM_DESTROY:	
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
			if (GPU::Instance != nullptr && GPU::Instance->SwapChain)
			{
				Platform::window->Width = LOWORD(lParam);
				Platform::window->Height = HIWORD(lParam);
				GPU::Instance->Backbuffer->Release();
				GPU::Instance->SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				ID3D11Texture2D* pBuffer;
				GPU::Instance->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
					(void**)&pBuffer);

				GPU::Instance->Device->CreateRenderTargetView(pBuffer, NULL,
					&GPU::Instance->Backbuffer);
				pBuffer->Release();

				GPU::Instance->Context->OMSetRenderTargets(1, &GPU::Instance->Backbuffer, NULL);

				D3D11_VIEWPORT vp;
				vp.Width = Platform::window->Width;
				vp.Height = Platform::window->Height;
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = 0;
				vp.TopLeftY = 0;
				GPU::Instance->Context->RSSetViewports(1, &vp);
			}
			return 1;
	}
	
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

std::shared_ptr<Window> Window::Create(std::string title,int width,int height)
{
	std::wstring temp = std::wstring(title.begin(), title.end());
	LPCWSTR titleLpcwstr = temp.c_str();

	std::shared_ptr<Window> window = std::make_shared<Window>();
	window->Width = width;
	window->Height = height;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW;	
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;	
	wc.cbWndExtra = NULL;	
	wc.hInstance = Platform::hInstance;	
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = window->WndClassName;	
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	RECT wr = { 0, 0, width,height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	window->Hwnd = CreateWindowExW(
		NULL,	
		window->WndClassName,	
		titleLpcwstr,
		WS_OVERLAPPEDWINDOW,	
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,	
		NULL,	
		Platform::hInstance,	
		NULL	
	);

	if (!window->Hwnd)	
	{
		
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
	}

	ShowWindow(window->Hwnd, SW_SHOW);	
	UpdateWindow(window->Hwnd);	
	return window;
}
