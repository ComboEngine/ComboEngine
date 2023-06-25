#include "pch.h"
#include "PointLight.h"
#include "../Core.h"


void PointLight::Update(Actor* actor)
{
}

void PointLight::Draw(Actor* actor)
{
	Light LightData{};
	LightData.Type = Point;
	LightData.Color = Color;
	LightData.Intensity = Intensity;
	LightData.Direction = actor->Position;
	Core::Scene.LightingData.push_back(LightData);
}

Component* PointLight::Clone()
{
	return new PointLight(*this);
}
