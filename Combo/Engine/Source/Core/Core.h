#pragma once

#include "Window.h"
#include "Platform.h"
#include "Context.h"
#include "Scripting.h"
#include "Actor.h"
#include "Event.h"
#include "Framebuffer.h"
#include "RenderStages.h"
#include "Project.h"
#include "GBuffer.h"
#include "Scene.h"
#include "PostFX.h"

enum RendererAPI {
	Null,
	DirectX11,
	Vulkan
};

class Core {
public:
	static RendererAPI RendererType;
	static Window* s_Window;
	static Platform* s_Platform;
	static Context* s_Context;
	static GBuffer* s_GBuffer;
	static Scripting* s_Scripting;
	static Project s_Project;

	static Event BeginPlayEvent;
	static Event UpdateEvent;
	static Event DrawEvent;
	static Event ExitEvent;
	static Event ImGuiDrawEvent;

	static Scene Scene;
	static PostFXRenderer PostFX;

	static bool ShouldExit;

	static void RequestExit();
	static int Init();
};