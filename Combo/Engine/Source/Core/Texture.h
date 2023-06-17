#pragma once
#include "Scope.h"
#include "pch.h"

class Texture {
public:
	void* MipData;
	int Width;
	int Height;
	static void Create(Scope<Texture>& Obj,void* mipData, int Width, int Height);

	virtual void Init(void* mipData, int Width, int Height) = 0;
	virtual std::string GetApiName() = 0;
};