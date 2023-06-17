#pragma once
#include "pch.h"
#include "Scope.h"

enum FramebufferTarget {
	Color,
	Depth
};

class Framebuffer {
public:
	FramebufferTarget Target;
	int Width;
	int Height;
	static void Create(Scope<Framebuffer>& Obj, int Width, int Height,FramebufferTarget Target);

	//API Functions
	virtual void Init() = 0;
	virtual void* GetImage() = 0;
	virtual std::string GetApiName() = 0;
};