#pragma once
#include "../Panel.h"
#include <Core/Assets/Asset.h>

class MaterialPropertiesPanel : public Panel {
public:
	Asset* material;
	Asset* diffuseTexture;
	virtual std::string GetName();
	virtual void Draw();
};