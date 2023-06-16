#pragma once

#include "Scope.h"
#include "Window.h"
#include "Platform.h"
#include "Context.h"
#include "Event.h"

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

	static Event BeginPlayEvent;
	static Event UpdateEvent;
	static Event DrawEvent;
	static Event ExitEvent;

	static bool ShouldExit;

	static void RequestExit();
	static int Init();
};