#pragma once

#include <Core/Actor.h>

class Editor {
public:
	static bool MouseHooked;
	static void Init();
	static void RenderComponent(std::string name,Component* component);
};