#pragma once
#include "pch.h"
#include <map>

class Input {
public:
	static std::map<int, bool> Keys;
	static glm::vec2 LastMousePosition;
	static glm::vec2 CurrentMousePosition;
	static bool IsKeyDown(int key);
	static glm::vec2 GetMouseDelta();
};