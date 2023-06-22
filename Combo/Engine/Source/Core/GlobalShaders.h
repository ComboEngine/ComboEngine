#pragma once
#include "Shader.h"

enum GlobalShader {
	Render3D,
	PostFX
};

class GlobalShaders {
private:
	static Shader* Render3D;
	static Shader* PostFX;
public:
	static void Init();
	static Shader* GetShader(GlobalShader shader);
};