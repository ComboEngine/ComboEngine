#pragma once
#include "pch.h"
#include "Scope.h"
#include "VertexStruct.h"

class ShaderDataBuffer {
public:
	static void Create(ShaderDataBuffer** shaderDataBuffer,size_t Size);

	//API Functions
	virtual void Init(size_t Size) = 0;
	virtual void Update(const void* Data) = 0;
	virtual std::string GetApiName() = 0;
};