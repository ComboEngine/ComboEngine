#include "pch.h"
#include "Asset.h"
#include "MeshAsset.h"
#include "../Core.h"
void Asset::Create(Scope<Asset>& Obj, std::string path)
{
	std::string extension = std::filesystem::u8path(path).extension().string();

	if (extension == ".cbmesh") {
		Obj.Set(new MeshAsset());
	}

	Obj.Get()->ReadFromFile(path);
	Core::s_Project.Assets[Obj.Get()->uuid] = Obj.Get();
	Obj.Get()->path = path;
}

void Asset::Import(Scope<Asset>& Obj, std::string filePath, std::string assetPath, std::any ImportSettings)
{
	std::string extension = std::filesystem::u8path(filePath).extension().string();

	if (extension == ".fbx" || extension == ".obj") {
		Obj.Set(new MeshAsset());
	}

	Obj.Get()->uuid = uuid::generate_uuid_v4();

	Obj.Get()->ImportToFile(filePath,assetPath,ImportSettings);
	Core::s_Project.Assets[Obj.Get()->uuid] = Obj.Get();
	Obj.Get()->path = assetPath;
}
