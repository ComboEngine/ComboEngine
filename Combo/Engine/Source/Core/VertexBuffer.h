#pragma once
#include "pch.h"
#include "Scope.h"
#include "VertexStruct.h"

class VertexBuffer {
public:
	static void Create(Scope<VertexBuffer>& vertexBuffer,std::vector<Vertex> Vertices);

	//API Functions
	virtual void Init(std::vector<Vertex> Vertices) = 0;
	virtual std::string GetApiName() = 0;
};