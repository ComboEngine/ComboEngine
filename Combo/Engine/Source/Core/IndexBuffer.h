#pragma once
#include "pch.h"
#include "Scope.h"
#include "VertexStruct.h"

class IndexBuffer {
public:
	static void Create(Scope<IndexBuffer>& indexBuffer,std::vector<int> Indices);

	//API Functions
	virtual void Init(std::vector<int> Indices) = 0;
	virtual std::string GetApiName() = 0;
};