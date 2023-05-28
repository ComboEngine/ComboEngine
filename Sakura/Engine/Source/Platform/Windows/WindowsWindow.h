#pragma once

#include <Engine/EngineHeaders.h>

class Window {
public:
	LPCTSTR WndClassName = L"SakuraEngine";
	HWND Hwnd = NULL;
	int Width;
	int Height;
	static sakura_ptr<Window> Create(sakura_string title, int width, int height);
};