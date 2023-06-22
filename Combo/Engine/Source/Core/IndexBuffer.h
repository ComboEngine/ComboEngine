#pragma once
#include "pch.h"
#include "Scope.h"
#include "VertexStruct.h"

class IndexBuffer {
public:
	int Size;
	static void Create(IndexBuffer** indexBuffer,std::vector<uint32_t> Indices);

	//API Functions
	virtual void Init(std::vector<uint32_t> Indices) = 0;
	virtual std::string GetApiName() = 0;
};