#pragma once
#include "pch.h"
#include "EngineUtils.h"
#include <Platform/Platform.h>
#include <Utility/Version.h>
#include <World/World.h>

class Material;
class GPUFramebuffer;

class Engine {
public:
	static Logger logger;
	static Version version;
	static bool shouldExit;
	static std::vector<std::shared_ptr<Material>> Materials;
	static std::shared_ptr<GPUFramebuffer> Color;
	static std::shared_ptr<GPUFramebuffer> Depth;

	//Engine entry point
	static int Main(std::vector<std::string> args);
	static bool ShouldExit();
	static void RequestExit();

	static void OnStart();
	static void OnUpdate();
	static void OnDraw();
	static void OnExit();
};