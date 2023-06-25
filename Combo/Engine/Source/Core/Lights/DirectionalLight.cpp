#include "pch.h"
#include "DirectionalLight.h"
#include "../Core.h"


void DirectionalLight::Update(Actor* actor)
{
}

void DirectionalLight::Draw(Actor* actor)
{
	Light LightData{};
	LightData.Type = Directional;
	LightData.Color = Color;
	LightData.Intensity = Intensity;
	LightData.Direction = actor->Position;
	Core::Scene.LightingData.push_back(LightData);
}

Component* DirectionalLight::Clone()
{
	return new DirectionalLight(*this);
}
