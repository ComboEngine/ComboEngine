#pragma once
#include "Actor.h"
#include "Mesh.h"

class Renderer : public Component {
public:
	Scope<Mesh> mesh;
	virtual void Init();
};