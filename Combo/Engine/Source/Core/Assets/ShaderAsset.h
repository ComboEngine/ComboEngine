#pragma once
#include "Asset.h"
#include <Core/Shader.h>

class ShaderAsset : public Asset {
public:
	Shader* Handle;
	virtual void ImportFromOriginal(std::string BinaryPath);
	virtual void CreateEmpty();
	virtual void ImportFromEngineType();
	virtual std::string GetType();
	virtual std::any GetHandle();
	virtual void WriteEngineFormatToStream(std::ofstream stream, std::string BinaryPath);
	virtual void Save();
};