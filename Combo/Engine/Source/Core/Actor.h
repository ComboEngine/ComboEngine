#pragma once
#include "pch.h"
#include "Scope.h"

class Actor;

class Component {
public:
	virtual void Update(Scope<Actor> actor) = 0;
	virtual void Draw(Scope<Actor> actor) = 0;
	virtual std::string GetName() = 0;
};

class Actor {
public:
	bool ActorSelected = false;
	std::string Name = "Actor";
	std::vector<Component*> Components;
	glm::vec3 Position = glm::vec3(0,0,0);
	glm::vec3 Orientation = glm::vec3(0,0,0);
	glm::vec3 Scale = glm::vec3(1,1,1);
	static void Create(Scope<Actor>& Obj);
	void AddComponent(Component* component);
};