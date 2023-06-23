#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"

class Renderer : public Component {
public:
	//Editor side
	std::string MeshUUIDBuffer;
	//End Editor side

	Asset* mesh;
	Asset* material;
	virtual void Update(Actor* actor);
	virtual void Draw(Actor* actor);
	virtual std::string GetName() { return "Renderer"; }
};