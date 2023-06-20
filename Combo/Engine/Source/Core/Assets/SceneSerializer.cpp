#include "pch.h"
#include "../Core.h"
#include "SceneSerializer.h"
#include <nlohmann.h>
#include <fstream>


void SceneSerializer::LoadProject(std::string name)
{
}

void SceneSerializer::CreateProject(std::string name)
{
	nlohmann::json j;
	j["Name"] = name;

	std::vector<nlohmann::json> assets;

	for (const auto& a : Core::s_Project.Assets) {
		nlohmann::json asset;
		asset["UUID"] = a.first;
		asset["Type"] = a.second->GetType();
		asset["OriginalPath"] = a.second->path;
		asset["Name"] = a.second->GetName();

		assets.push_back(asset);
	}

	j["Assets"] = assets;

	std::ofstream file(name + ".cbproject");
	file << j.dump(4);
	file.close();
}
