#pragma once
#include "pch.h"
#include "Assets/Asset.h"

class Scene {
public:

};

class Project {
public:
	std::unordered_map<std::string, Asset*> Assets;
	std::vector<Scene> Scenes;
	std::vector<Asset*> GetAssetsByType(std::string type);
};