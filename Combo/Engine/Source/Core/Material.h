#pragma once

#include "Texture.h"
#include "Shader.h"

struct MaterialColor {
	Texture* ColorTexture;
	glm::vec4 Color;
	bool UseTexture = false;
	static MaterialColor FromTexture(Texture* Texture) {
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
	Shader* Shader;
	static void Create(Material** Obj);
};