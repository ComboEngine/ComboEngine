#pragma once
#include "pch.h"
#include "../Mesh.h"

class MeshSerializer {
public:
	static void Read(Scope<Mesh>& mesh, std::string path);
	static void Import(Scope<Mesh>& mesh, std::string path,std::string assetPath);
};