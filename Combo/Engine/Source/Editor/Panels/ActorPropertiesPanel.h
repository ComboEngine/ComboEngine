#pragma once
#include "../Panel.h"
#include <Core/Actor.h>

class ActorPropertiesPanel : public Panel {
public:
	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	void RenderComponent(std::string name, Component* component);

	virtual std::string GetName();
	virtual void Draw();
};