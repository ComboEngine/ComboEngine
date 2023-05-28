#include "WindowsWindow.h"
#include "WindowsPlatform.h"


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
			return 0;
	}
	
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

sakura_ptr<Window> Window::Create(sakura_string title,int width,int height)
{
	wstring temp = wstring(title.begin(), title.end());
	LPCWSTR titleLpcwstr = temp.c_str();

	sakura_ptr<Window> window = make_shared<Window>();
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

	window->Hwnd = CreateWindowExW(
		NULL,	
		window->WndClassName,	
		titleLpcwstr,
		WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT, CW_USEDEFAULT,	
		width,	
		height,	
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
