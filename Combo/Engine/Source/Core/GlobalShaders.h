#pragma once
#include "Shader.h"

enum GlobalShader {
	Render3D
};

class GlobalShaders {
private:
	static Scope<Shader> Render3D;
public:
	static void Init();
	static Scope<Shader> GetShader(GlobalShader shader);
};