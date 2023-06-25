#pragma once

enum LightType {
	Directional,
	Point
};
class Light {
public:
	LightType Type;
	float Intensity;
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Direction;
};