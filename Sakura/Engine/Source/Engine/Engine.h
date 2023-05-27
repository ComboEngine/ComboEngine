#pragma once
#include "EngineHeaders.h"
#include "EngineUtils.h"
#include <Utility/Version.h>

class Engine {
public:
	static Logger logger;
	static Version version;

	//Engine entry point
	static int Main(sakura_array<sakura_string> args);
	static bool ShouldExit();

	static void OnStart();
	static void OnUpdate();
	static void OnDraw();
	static void OnExit();
};