#pragma once
#include <Engine/EngineHeaders.h>
#include "Asset.h"
#include <fstream>
#include <Utility/JSON.h>
#include <Utility/UUID.h>

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

		if (asset->type() == "Material") {
			nlohmann::json j = nlohmann::json::parse(LoadBytes(path));

			asset->name = j["name"];
			asset->uuid = j["uuid"];
			asset->Material = Material::Create();
		}

		return asset;
	}
};