#pragma once
#include "pch.h"


class Actor;

class Component {
public:
	virtual void Update(Actor* actor) = 0;
	virtual void Draw(Actor* actor) = 0;
	virtual std::string GetName() = 0;
};

class Actor {
public:
	bool ActorSelected = false;
	std::string UUID;
	std::string Name = "Actor";
	std::vector<Component*> Components;
	glm::vec3 Position = glm::vec3(0,0,0);
	glm::vec3 Orientation = glm::vec3(0, 0, 0);
	glm::vec3 Scale = glm::vec3(1,1,1);
	static void Create(Actor** Obj);
	void AddComponent(Component* component);
};