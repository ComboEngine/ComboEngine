#include "pch.h"
#include "Asset.h"
#include "MaterialAsset.h"
#include "MeshAsset.h"
#include "TextureAsset.h"
#include <Core/Core.h>

void Asset::New(Asset** Obj, AssetType type, std::string ProjectPath)
{
	switch (type) {
	case AssetType::MaterialAssetType: *Obj = new MaterialAsset(); break;
	}

	if (*Obj != nullptr) {
		Asset* Ptr = *Obj;
		Ptr->UUID = uuid::generate_uuid_v4();
		Ptr->OSPath = GenerateOSPath(ProjectPath);
		Ptr->Name = std::filesystem::u8path(Ptr->OSPath).filename().string().substr(0, std::filesystem::u8path(Ptr->OSPath).filename().string().find_last_of("."));
		Ptr->ProjectPath = ProjectPath;
		Ptr->CreateEmpty();
		Core::s_Project.Assets[Ptr->UUID] = Ptr;
	}
}

void Asset::ImportFromCb(Asset** Obj, std::string OSCbPath, std::string CurrentEditorFolder)
{
	std::string Extension = std::filesystem::u8path(OSCbPath).extension().string();
	if (Extension == ".cbmaterial") { *Obj = new MaterialAsset(); }
	if (Extension == ".cbmesh") { *Obj = new MeshAsset(); }
	if (Extension == ".cbtexture") { *Obj = new TextureAsset(); }

	Asset* Ptr = *Obj;
	Ptr->OSPath = OSCbPath;
	Ptr->ProjectPath = CurrentEditorFolder + std::filesystem::u8path(OSCbPath).filename().string();
	Ptr->ImportFromEngineType();
	Core::s_Project.Assets[Ptr->UUID] = Ptr;
}

void Asset::ImportFromBinary(Asset** Obj, std::string BinaryPath, std::string ProjectSpacePath, std::string CurrentEditorFolder)
{
	std::string Extension = std::filesystem::u8path(BinaryPath).extension().string();
	if (Extension == ".fbx" || Extension == ".obj" || Extension == ".dae") { *Obj = new MeshAsset(); }
	if (Extension == ".png" || Extension == ".jpg") { *Obj = new TextureAsset(); }

	Asset* Ptr = *Obj;
	Ptr->UUID = uuid::generate_uuid_v4();
	Ptr->OSPath = GenerateOSPath(ProjectSpacePath);
	Ptr->Name = std::filesystem::u8path(Ptr->OSPath).filename().string().substr(0, std::filesystem::u8path(Ptr->OSPath).filename().string().find_last_of("."));
	Ptr->ProjectPath = ProjectSpacePath;
	Ptr->ImportFromOriginal(BinaryPath);
	Core::s_Project.Assets[Ptr->UUID] = Ptr;
}

std::string Asset::GenerateOSPath(std::string ProjectPath)
{
	return Core::s_Project.ProjectSpaceDirectory + ProjectPath;
}
