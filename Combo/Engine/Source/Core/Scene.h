#pragma once
#include "pch.h"
#include "Actor.h"


class Scene {
public:
	glm::vec3 DefaultCameraPosition = glm::vec3(0,0,0);
	glm::vec3 DefaultCameraOrientation = glm::vec3(0, 0, 0);
	std::vector<Actor*> Actors;
};