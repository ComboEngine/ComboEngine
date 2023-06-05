#pragma once
#include "pch.h"
#include "WindowsWindow.h"

class Platform {
public:
	static HINSTANCE hInstance;
	static std::shared_ptr<Window> window;
	static MSG msg;
	static void PreInit(HINSTANCE hInstance);
	static void Init();
	static void OnUpdate();
};