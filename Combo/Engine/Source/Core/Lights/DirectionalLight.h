#pragma once
#include "../Actor.h"
#include "../Light.h"

class DirectionalLight : public Component {
public:
	float Intensity = 1.0f;
	glm::vec3 Color = glm::vec3(1,1,1);
	virtual void Update(Actor* actor);
	virtual void Draw(Actor* actor);
	virtual Component* Clone();
	virtual std::string GetName() { return "Directional Light"; }
};