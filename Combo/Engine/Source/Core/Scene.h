#pragma once
#include "pch.h"
#include "Actor.h"
#include "Light.h"


class Scene {
public:
	std::vector<Light> LightingData;
	glm::vec3 DefaultCameraPosition = glm::vec3(0,0,0);
	glm::vec3 DefaultCameraOrientation = glm::vec3(0, 0, 0);
	std::vector<Actor*> Actors;
	std::vector<Actor*> ActorDeleteQueue;
	void RemoveActor(Actor* actor);
	void EndScene();
};