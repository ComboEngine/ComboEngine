#pragma once

#include <Core/Actor.h>

enum EditorViewMode {
	Position,
	Normal,
	Diffuse,
	FinalBuffer
};

class Editor {
public:
	static EditorViewMode ViewMode;
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