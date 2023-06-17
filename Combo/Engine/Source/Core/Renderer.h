#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"

class Renderer : public Component {
public:
	Scope<Mesh> mesh;
	Scope<Material> material;
	virtual void Update(Scope<Actor> actor);
	virtual void Draw(Scope<Actor> actor);
};