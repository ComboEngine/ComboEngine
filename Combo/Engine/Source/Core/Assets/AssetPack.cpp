#include "pch.h"
#include "AssetPack.h"
#include <Core/Core.h>

void AssetPack::Save(std::string path)
{
	std::ofstream file;
	file.open(path.c_str(), std::ios::binary | std::ios::out);

	std::string ver = "Combo.Engine 0.1.34";
	uint32_t size = ver.size();
	file.write((const char*)&size, sizeof(uint32_t));
	file.write(ver.data(),size);

	uint32_t assetsSize = Core::s_Project.Assets.size();
	file.write((const char*)&assetsSize, sizeof(uint32_t));

	for (const auto& asset : Core::s_Project.Assets) {
		if (asset.second != nullptr) {
			std::ifstream a;
			std::cout << asset.second->OSPath.c_str() << std::endl;
			a.open(asset.second->OSPath.c_str(), std::ios::binary);
			a.seekg(0);
			file << a.tellg();
			file << a.rdbuf();
		}
	}

	file.close();
}

void AssetPack::Load(std::string path)
{
	std::ifstream file;
	file.open(path.c_str(), std::ios::binary);
}
