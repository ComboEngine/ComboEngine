#pragma once

#include "Scope.h"
#include "Window.h"
#include "Platform.h"
#include "Context.h"

enum RendererAPI {
	Null,
	DirectX11
};

class Core {
public:
	static RendererAPI RendererType;
	static Scope<Window> s_Window;
	static Scope<Platform> s_Platform;
	static Scope<Context> s_Context;
	static int Init();
};