#pragma once
#include "pch.h"
#include <Core/Scope.h>
#include "UUID.h"

class Asset {
public:
	std::string uuid;
	std::string path;

	static void Create(Asset** Obj,std::string path);
	static void Import(Asset** Obj, std::string filePath, std::string assetPath, std::any ImportSettings);

	virtual void ReadFromFile(std::string path) = 0;
	virtual void ImportToFile(std::string filePath, std::string assetPath,std::any ImportSettings) = 0;
	virtual std::any GetHandle() = 0;
	virtual std::string GetName() = 0;
	virtual std::string GetType() = 0;
	std::string GetUUID() { return uuid; }
};