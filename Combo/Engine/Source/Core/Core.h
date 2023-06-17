#pragma once

#include "Scope.h"
#include "Window.h"
#include "Platform.h"
#include "Context.h"
#include "Scripting.h"
#include "Actor.h"
#include "Event.h"
#include "Framebuffer.h"

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
	static Scope<Scripting> s_Scripting;
	static Scope<Framebuffer> s_Color;

	static Event BeginPlayEvent;
	static Event UpdateEvent;
	static Event DrawEvent;
	static Event ExitEvent;
	static Event ImGuiDrawEvent;

	static std::vector<Scope<Actor>> Actors;

	static bool ShouldExit;

	static void RequestExit();
	static int Init();
};