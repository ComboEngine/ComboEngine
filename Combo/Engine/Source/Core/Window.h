#pragma once
#include "pch.h"
#include "Scope.h"

struct WindowSpecification {
	std::string Title = "Combo";
	int Width = 1920;
	int Height = 1080;
};

class Window {
public:
	WindowSpecification Specification;
	static void Create(Window** window,WindowSpecification Specification);

	//API Functions
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual const void* GetPlainWindow() = 0;
	virtual void LockCursor(bool lock) = 0;
	virtual std::string GetApiName() = 0;
};