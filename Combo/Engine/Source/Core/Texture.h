#pragma once
#include "Scope.h"
#include "pch.h"

class Texture {
public:
	static void Create(Scope<Texture>& Obj,void* mipData, int Width, int Height);

	virtual void Init(void* mipData, int Width, int Height) = 0;
	virtual std::string GetApiName() = 0;
};