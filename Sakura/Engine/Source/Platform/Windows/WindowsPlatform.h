#pragma once

#include "WindowsWindow.h"
#include "IncludeWindowsHeaders.h"

class Platform {
public:
	static HINSTANCE hInstance;
	static sakura_ptr<Window> window;
	static MSG msg;
	static void PreInit(HINSTANCE hInstance);
	static void Init();
	static void OnUpdate();
};