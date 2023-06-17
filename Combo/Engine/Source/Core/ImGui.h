#pragma once
#include "Scope.h"
class ImGuiAdapter {
public:
	static void Init();
	static void StartFrame();
	static void EndFrame();
};