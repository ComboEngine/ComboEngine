#pragma once
#include "pch.h"
#include "../Texture.h"

class TextureSerializer {
public:
	static void Read(Scope<Texture>& texture, std::string path);
	static void Import(Scope<Texture>& texture, std::string path,std::string assetPath);
};