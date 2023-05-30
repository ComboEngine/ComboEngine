#pragma once

#include "GPUShader.h"
#include "Mesh.h"

class GPUShader;

class GPURenderData {
public:
	sakura_ptr<GPUShader> Shader;
	sakura_ptr<Mesh> Mesh;
};