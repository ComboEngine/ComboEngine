#pragma once
#include <Engine/EngineHeaders.h>
#include "Asset.h"
#include <fstream>

class AssetManager {
public:
	static void LoadAssetPack(sakura_string path);
	static void SaveAssetPack(sakura_string path);
	static sakura_string LoadBytes(sakura_string path) {
		std::ifstream t(path);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		return str;
	}
	template<class T> static sakura_ptr<T> GetAsset(sakura_string path) {
		sakura_ptr<T> asset = make_shared<T>();
		asset->Init(LoadBytes(path));
		return asset;
	}
};