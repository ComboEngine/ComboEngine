#pragma once
#include "Asset.h"
#include <Core/Mesh.h>

class MeshAsset : public Asset {
public:
	Mesh* Handle;
	virtual void ImportFromOriginal(std::string BinaryPath);
	virtual void CreateEmpty();
	virtual void ImportFromEngineType();
	virtual std::string GetType();
	virtual std::any GetHandle();
};