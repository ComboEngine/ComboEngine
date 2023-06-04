#pragma once

#include <pch.h>

class Window {
public:
	LPCTSTR WndClassName = L"SakuraEngine";
	HWND Hwnd = NULL;
	int Width;
	int Height;
	static std::shared_ptr<Window> Create(std::string title, int width, int height);
};