#pragma once
#include "pch.h"
#include "Scope.h"

struct ContextSpecification {
	std::string Title = "Combo";
	int Width = 1920;
	int Height = 1080;
};

class Context {
public:
	ContextSpecification Specification;
	static void Create(Scope<Context>& Obj, ContextSpecification Specification);

	//API Functions
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual std::string GetApiName() = 0;
};