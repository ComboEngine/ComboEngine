#pragma once

#include "Scope.h"
#include "Window.h"
#include "Platform.h"

enum RendererAPI {
	Null,
	DirectX11
};

class Core {
public:
	static RendererAPI RendererType;
	static Scope<Window> s_Window;
	static Scope<Platform> s_Platform;
	static int Init();
};