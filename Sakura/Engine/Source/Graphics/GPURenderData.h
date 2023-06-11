#pragma once

#include "GPUShader.h"
#include "Mesh.h"
#include <Renderer/Material.h>

class GPUShader;
class Material;


struct GPURenderData {
	std::shared_ptr<Material> Material;
	std::shared_ptr<Mesh> Mesh;
	glm::mat4 Matrix;
};