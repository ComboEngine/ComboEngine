#pragma once
#include <Engine/EngineHeaders.h>

class AssetManager {
public:
	static void LoadAssetPack(sakura_string path);
	static void SaveAssetPack(sakura_string path);
};