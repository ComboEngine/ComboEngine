#pragma once
#include "Asset.h"
#include "../Mesh.h"

class MaterialAsset : public Asset {
public:
	Material* Handle;
	std::string Name;

	virtual void ReadFromFile(std::string path);
	virtual void ImportToFile(std::string filePath, std::string assetPath, std::any ImportSettings);
	virtual std::any GetHandle();
	virtual std::string GetName();
	virtual std::string GetType();
};