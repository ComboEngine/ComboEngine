#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"
#include "Assets/MeshAsset.h"

class Renderer : public Component {
public:
	//Editor side
	std::string MeshUUIDBuffer;
	//End Editor side

	Asset* mesh;
	Scope<Material> material;
	virtual void Update(Scope<Actor> actor);
	virtual void Draw(Scope<Actor> actor);
	virtual std::string GetName() { return "Renderer"; }
};