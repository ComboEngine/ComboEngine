#include "AssetManager.h"

#include <filesystem>
#include <fstream>
#include <Renderer/Material.h>
#include <Engine/Engine.h>


void AssetManager::LoadAssetPack(sakura_string path)
{

}

void AssetManager::SaveAssetPack(sakura_string path)
{
	for (sakura_ptr<Material> material : Engine::Materials) {
		
	}
}
