#pragma once
#include "pch.h"
#include "Scope.h"


class Shader {
public:
	static void Create(Scope<Shader>& shader, std::string VertexSource, std::string PixelSource);

	//API Functions
	virtual void Init(std::string VertexSource, std::string PixelSource) = 0;
	virtual std::string GetApiName() = 0;
};