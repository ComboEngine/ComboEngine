#pragma once
#include <Graphics/Mesh.h>
#include <Graphics/GPU.h>

class AssimpModelImporter {
public:
	static std::shared_ptr<Mesh> LoadMesh(std::string path);
};