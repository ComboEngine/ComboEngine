#pragma once
#include "UUID.h"

enum AssetType {
	MaterialAssetType,
	MeshAssetType
};

class Asset {
public:
	//Asset Universal ID
	std::string UUID;
	//Asset Name
	std::string Name;
	//Asset OS Path (Path to actual os asset file)
	std::string OSPath;
	//Asset Project Path (Path in project space)
	std::string ProjectPath;
	//Asset Handle
	

	static void New(Asset** Obj,AssetType type,std::string ProjectPath);
	static void ImportFromCb(Asset**, std::string OSCbPath, std::string CurrentEditorFolder);
	static void ImportFromBinary(Asset**, std::string BinaryPath, std::string ProjectSpacePath, std::string CurrentEditorFolder);

	static std::string GenerateOSPath(std::string ProjectPath);
	

	//Original = mesh -> fbx,obj, texture -> png, jpg
	virtual void ImportFromOriginal(std::string BinaryPath) = 0;
	virtual void CreateEmpty() = 0;
	virtual void ImportFromEngineType() = 0;
	virtual void Save() = 0;
	virtual std::any GetHandle() = 0;
	virtual std::string GetType() = 0;
};