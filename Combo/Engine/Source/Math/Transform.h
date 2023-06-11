#pragma once
#include "pch.h"

class Transform {
public:
	glm::vec3 Position;
	glm::quat Orientation;
	glm::vec3 Scale;
	glm::mat4 CalculateMatrix();
	void Rotate(glm::quat orientation);
	void Move(glm::vec3 position);
};