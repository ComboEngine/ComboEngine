#pragma once
#include "pch.h"

enum TargetType {
	TEXT,
	RECTANGLE,
	TEXTUREDRECTANGLE
};

struct DrawTarget {
	TargetType Type;
	std::unordered_map<std::string,std::any> Data;
};

class Render2D {
public:
	static std::vector<DrawTarget> DrawTargets;
	static void RenderImGui();
	static void RenderRect(int x, int y, int width, int height,glm::vec4 color);
	static void RenderText(std::string text, int x, int y, glm::vec4 color);
	static void RenderTexturedRect(int x, int y, int width, int height, void* texture);
};