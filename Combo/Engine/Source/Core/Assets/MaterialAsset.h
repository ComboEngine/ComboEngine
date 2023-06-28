#pragma once
#include "Asset.h"
#include <Core/Material.h>

class MaterialAsset : public Asset{
public:
	Material* Handle;
	virtual void ImportFromOriginal(std::string BinaryPath);
	virtual void CreateEmpty();
	virtual void ImportFromEngineType();
	virtual std::string GetType();
	virtual std::any GetHandle();
	virtual void WriteEngineFormatToStream(std::ofstream stream, std::string BinaryPath);
	virtual void Save();
};