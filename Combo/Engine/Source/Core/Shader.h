#pragma once
#include "pch.h"



class Shader {
public:
	static void Create(Shader** shader, std::string VertexSource, std::string PixelSource);

	//API Functions
	virtual void Init(std::string VertexSource, std::string PixelSource) = 0;
	virtual std::string GetApiName() = 0;
};