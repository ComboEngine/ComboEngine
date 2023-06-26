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
	static std::string GetShaderPathByApi(std::string name,bool pixel);
public:
	static void Init();
	static Shader* GetShader(GlobalShader shader);
};