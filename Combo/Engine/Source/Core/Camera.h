#pragma once
#include "pch.h"

class Camera {
public:
	static int ProjectionWidth;
	static int ProjectionHeight;
	static glm::vec3 Position;
	static glm::vec3 Orientation;
	static void Drone();
	static glm::mat4 CalculateViewMatrix();
	static glm::mat4 CalculateProjectionMatrix();
};