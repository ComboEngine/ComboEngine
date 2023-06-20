#pragma once

#include <Core/Actor.h>

class Editor {
public:
	static bool MouseHooked;
	static bool ShowImport;
	static bool ShowImportExisting;
	static std::string ToImportPathBuffer;
	static std::string AssetPathBuffer;
	static void Init();
	static void ImportAsset();
	static void ImportExistingAsset();
	static void OnDrop(std::vector<std::string> paths);
	static void RenderComponent(std::string name, Component* component);
};