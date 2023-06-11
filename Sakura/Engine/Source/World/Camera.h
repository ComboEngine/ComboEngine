#pragma once
#include "pch.h"
#include "Actor.h"

class Transform;
class Camera {
public:
	std::shared_ptr<Transform> transform;
	void UpdateCamera();
	glm::mat4 CalculateMatrix();
};