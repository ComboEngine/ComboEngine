#pragma once
#include "pch.h"
#include "Scope.h"

class Component {
public:
	virtual void Init() = 0;
};

class Actor {
public:
	std::vector<Component*> Components;
	glm::vec3 Position = glm::vec3(0,0,0);
	glm::quat Orientation = glm::quat(glm::vec3(0,0,0));
	glm::vec3 Scale = glm::vec3(1,1,1);
	static void Create(Scope<Actor>& Obj);
	void AddComponent(Component* component);
};