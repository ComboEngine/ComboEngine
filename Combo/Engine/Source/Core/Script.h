#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"

class Script : public Component {
public:
	std::string ComponentName;
	Scope<Mesh> mesh;
	Scope<Material> material;
	virtual void Update(Scope<Actor> actor);
	virtual void Draw(Scope<Actor> actor);
	virtual std::string GetName() { return "Script"; }
};