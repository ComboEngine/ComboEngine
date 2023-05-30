#pragma once
#include "EngineHeaders.h"
#include "EngineUtils.h"
#include <Platform/Platform.h>
#include <Utility/Version.h>
#include <World/World.h>

class Material;

class Engine {
public:
	static Logger logger;
	static Version version;
	static bool shouldExit;
	static sakura_array<sakura_ptr<Material>> Materials;

	//Engine entry point
	static int Main(sakura_array<sakura_string> args);
	static bool ShouldExit();
	static void RequestExit();

	static void OnStart();
	static void OnUpdate();
	static void OnDraw();
	static void OnExit();
};