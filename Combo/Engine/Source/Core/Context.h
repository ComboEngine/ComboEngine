#pragma once
#include "pch.h"
#include "Scope.h"
#include "Framebuffer.h"
#include "Pipeline.h"

struct ContextSpecification {
	
};

class Context {
public:
	ContextSpecification Specification;
	static void Create(Context** Obj, ContextSpecification Specification);

	//API Functions
	virtual void Init() = 0;
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void Draw(Pipeline pipeline) = 0;
	virtual void SetClearColor(glm::vec3 color) = 0;
	virtual std::string GetApiName() = 0;
};