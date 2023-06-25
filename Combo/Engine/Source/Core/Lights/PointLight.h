#pragma once
#include "../Actor.h"
#include "../Light.h"

class PointLight : public Component {
public:
	float Intensity = 1.0f;
	glm::vec3 Color = glm::vec3(1, 1, 1);
	virtual void Update(Actor* actor);
	virtual void Draw(Actor* actor);
	virtual std::string GetName() { return "Point Light"; }
};