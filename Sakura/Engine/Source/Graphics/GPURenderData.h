#pragma once

#include "GPUShader.h"
#include "Mesh.h"
#include <Renderer/Material.h>

class GPUShader;
class Material;


class GPURenderData {
public:
	sakura_ptr<Material> Material;
	sakura_ptr<Mesh> Mesh;
};