#include "pch.h"
#include <Graphics/Mesh.h>


class MeshSerializer {
public:
	static void Save(std::shared_ptr<Mesh> mesh,std::string path);
	static std::shared_ptr<Mesh> Read(std::string path);
};