#pragma once

#include <Engine/EngineHeaders.h>
#include <Renderer/Material.h>


class Asset {
public:
	sakura_string uuid;
	sakura_string name;
};

class MaterialAsset : public Asset{
public:
	sakura_string type() { return "Material"; }
	sakura_ptr<Material> Material = nullptr;
};