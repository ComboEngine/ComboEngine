#include "pch.h"
#include "Project.h"

std::vector<Asset*> Project::GetAssetsByType(std::string type)
{
	std::vector<Asset*> assets;
	for (const auto& a : this->Assets) {
		if (a.second->GetType() == type) {
			assets.push_back(a.second);
		}
	}

	return assets;
}