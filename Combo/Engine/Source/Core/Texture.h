#pragma once

#include "pch.h"
#include "Framebuffer.h"

class Texture {
public:
	static void Create(Texture** Obj,void* mipData, int Width, int Height);

	virtual void Init(void* mipData, int Width, int Height) = 0;
	virtual void ReadFromFramebuffer(Framebuffer* framebuffer)  = 0;
	virtual std::string GetApiName() = 0;
	virtual void* GetImGuiImage() = 0;
};