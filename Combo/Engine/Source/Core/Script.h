#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"

class Script : public Component {
public:
	std::string ComponentName;
	Mesh* mesh;
	Material* material;
	virtual void Update(Actor* actor);
	virtual void Draw(Actor* actor);
	virtual std::string GetName() { return "Script"; }
};