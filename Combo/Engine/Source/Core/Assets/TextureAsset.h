#pragma once
#include "Asset.h"
#include <Core/Texture.h>

class TextureAsset : public Asset {
public:
	Texture* Handle;
	virtual void ImportFromOriginal(std::string BinaryPath);
	virtual void CreateEmpty();
	virtual void ImportFromEngineType();
	virtual std::string GetType();
	virtual void WriteEngineFormatToStream(std::ofstream stream, std::string BinaryPath);
	virtual std::any GetHandle();
	virtual void Save();
};