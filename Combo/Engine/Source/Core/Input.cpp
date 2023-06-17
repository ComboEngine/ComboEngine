#include "pch.h"
#include "Input.h"

std::map<int, bool> Input::Keys;
glm::vec2 Input::LastMousePosition;
glm::vec2 Input::CurrentMousePosition;

bool Input::IsKeyDown(int key)
{
	return Keys[key];
}

glm::vec2 Input::GetMouseDelta()
{
	return LastMousePosition - CurrentMousePosition;
}
