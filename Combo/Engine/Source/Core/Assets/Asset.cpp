#include "pch.h"
#include "Asset.h"
#include "MeshAsset.h"
#include "MaterialAsset.h"
#include "../Core.h"
void Asset::Create(Asset** Obj, std::string path)
{
	std::string extension = std::filesystem::u8path(path).extension().string();

	if (extension == ".cbmesh") {
		*Obj = new MeshAsset();
	}

	if (extension == ".cbmaterial") {
		*Obj = new MaterialAsset();
	}

	Asset* ObjPtr = *Obj;
	ObjPtr->ReadFromFile(path);
	Core::s_Project.Assets[ObjPtr->uuid] = ObjPtr;
	ObjPtr->path = path;
}

void Asset::Import(Asset** Obj, std::string filePath, std::string assetPath, std::any ImportSettings)
{
	std::string extension = std::filesystem::u8path(filePath).extension().string();

	if (extension == ".fbx" || extension == ".obj") {
		*Obj = new MeshAsset();
	}

	Asset* ObjPtr = *Obj;
	ObjPtr->uuid = uuid::generate_uuid_v4();

	ObjPtr->ImportToFile(filePath,assetPath,ImportSettings);
	Core::s_Project.Assets[ObjPtr->uuid] = ObjPtr;
	ObjPtr->path = assetPath;
}
