#pragma once
#include "Scope.h"
#include "Texture.h"
#include "Shader.h"

struct MaterialColor {
	Scope<Texture> ColorTexture;
	glm::vec4 Color;
	bool UseTexture = false;
	static MaterialColor FromTexture(Scope<Texture> Texture) {
		MaterialColor color;
		color.ColorTexture = Texture;
		color.UseTexture = true;
		return color;
	}
	static MaterialColor FromColor(glm::vec4 Color) {
		MaterialColor color;
		color.Color = Color;
		color.UseTexture = false;
		return color;
	}
};

class Material {
public:
	bool CastShadow = true;
	bool Wireframe = false;
	MaterialColor Diffuse;
	Scope<Shader> Shader;
	static void Create(Scope<Material>& Obj);
};