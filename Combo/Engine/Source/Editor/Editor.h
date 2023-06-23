#pragma once

#include <Core/Actor.h>
#include "Panel.h"


class Editor {
public:
	static std::vector<Panel*> Panels;
	static glm::vec3 LightDir;
	static void Init();
	static void OnDrop(std::vector<std::string> paths);
};